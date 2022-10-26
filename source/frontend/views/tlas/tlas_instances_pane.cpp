//=============================================================================
// Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief  Implementation of the Instances pane on the TLAS tab.
//=============================================================================

#include "views/tlas/tlas_instances_pane.h"

#include "managers/message_manager.h"
#include "models/tlas/tlas_instances_model.h"
#include "views/widget_util.h"

TlasInstancesPane::TlasInstancesPane(QWidget* parent)
    : BasePane(parent)
    , ui_(new Ui::TlasInstancesPane)
    , tlas_index_(0)
    , instance_index_(UINT32_MAX)
    , data_valid_(false)
{
    ui_->setupUi(this);
    rra::widget_util::ApplyStandardPaneStyle(this, ui_->main_content_, ui_->main_scroll_area_);
    model_ = new rra::TlasInstancesModel(rra::kTlasInstancesNumWidgets);

    model_->InitializeModel(ui_->title_tlas_address_, rra::kTlasInstancesTlasBaseAddress, "text");

    // Initialize table.
    model_->InitializeTableModel(ui_->instances_table_, 0, rra::kTlasInstancesColumnCount);

    connect(ui_->search_box_, &QLineEdit::textChanged, model_, &rra::TlasInstancesModel::SearchTextChanged);
    connect(ui_->instances_table_, &QAbstractItemView::doubleClicked, this, &TlasInstancesPane::GotoBlasInstanceFromTableSelect);
    connect(&rra::MessageManager::Get(), &rra::MessageManager::TlasSelected, this, &TlasInstancesPane::SetTlasIndex);
    connect(&rra::MessageManager::Get(), &rra::MessageManager::InstanceSelected, this, &TlasInstancesPane::SetInstanceIndex);

    table_delegate_ = new TableItemDelegate();
    ui_->instances_table_->setItemDelegate(table_delegate_);

    // Set up a connection between the blas list being sorted and making sure the selected blas is visible.
    connect(model_->GetProxyModel(), &rra::TlasInstancesProxyModel::layoutChanged, this, &TlasInstancesPane::ScrollToSelectedInstance);

    // This event filter allows us to override right click to deselect all rows instead of select one.
    ui_->instances_table_->viewport()->installEventFilter(this);

    // Hide the column that does the instance index mapping.
    ui_->instances_table_->setColumnHidden(rra::kTlasInstancesColumnInstanceIndex, true);
}

TlasInstancesPane::~TlasInstancesPane()
{
    delete model_;
    delete table_delegate_;
}

void TlasInstancesPane::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        // Deselect rows when escape is pressed.
        ui_->instances_table_->selectionModel()->clearSelection();
        break;
    default:
        break;
    }

    BasePane::keyPressEvent(event);
}

void TlasInstancesPane::showEvent(QShowEvent* event)
{
    if (data_valid_ == false)
    {
        // Deselect any selected rows.
        QItemSelectionModel* selected_item = ui_->instances_table_->selectionModel();
        if (selected_item->hasSelection())
        {
            QModelIndexList item_list = selected_item->selectedIndexes();
            if (item_list.size() > 0)
            {
                for (const auto& item : item_list)
                {
                    ui_->instances_table_->selectionModel()->select(item, QItemSelectionModel::Deselect);
                }
            }
        }

        ui_->instances_table_->setSortingEnabled(false);
        bool instances = model_->UpdateTable(tlas_index_);
        ui_->instances_table_->setSortingEnabled(true);
        data_valid_ = true;
        if (instances)
        {
            // There are instances, so show the table.
            ui_->table_valid_switch_->setCurrentIndex(1);
        }
        else
        {
            // No instances in this TLAS, show an empty page.
            ui_->table_valid_switch_->setCurrentIndex(0);
        }

        // If there's a selected instance, highlight the row.
        bool row_selected = false;
        if (instance_index_ != UINT32_MAX)
        {
            const QModelIndex model_index = model_->GetTableModelIndex(instance_index_);
            if (model_index.isValid())
            {
                ui_->instances_table_->selectRow(model_index.row());
                ui_->instances_table_->scrollTo(model_index, QAbstractItemView::ScrollHint::PositionAtTop);
                row_selected = true;
            }
        }
        if (!row_selected)
        {
            ui_->instances_table_->selectRow(-1);
        }
    }

    BasePane::showEvent(event);
}

void TlasInstancesPane::OnTraceClose()
{
    data_valid_ = false;
    tlas_index_ = 0;
    ui_->search_box_->setText("");
}

bool TlasInstancesPane::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        if (mouse_event->button() == Qt::MouseButton::RightButton)
        {
            // Deselect all rows in BLAS table.
            ui_->instances_table_->selectionModel()->clearSelection();
            return true;
        }
    }

    // Standard event processing.
    return QObject::eventFilter(obj, event);
}

void TlasInstancesPane::GotoBlasInstanceFromTableSelect(const QModelIndex& index)
{
    if (index.isValid())
    {
        int32_t instance_index = model_->GetInstanceIndex(index);
        if (instance_index != -1)
        {
            auto address = model_->GetInstanceAddress(instance_index);

            // First, switch the pane so the UI is initialized.
            emit rra::MessageManager::Get().PaneSwitchRequested(rra::kPaneIdTlasViewer);

            // Emit a message indicating that a BLAS instance has been double-clicked.
            emit rra::MessageManager::Get().InstancesTableDoubleClicked(tlas_index_, address.blas_index, address.instance_index);
        }
    }
}

void TlasInstancesPane::SetTlasIndex(uint64_t tlas_index)
{
    if (tlas_index != tlas_index_)
    {
        tlas_index_ = tlas_index;
        data_valid_ = false;
    }
}

void TlasInstancesPane::SetInstanceIndex(uint32_t instance_index)
{
    if (instance_index != instance_index_)
    {
        instance_index_ = instance_index;
        data_valid_     = false;
    }
}

void TlasInstancesPane::ScrollToSelectedInstance()
{
    QItemSelectionModel* selected_item = ui_->instances_table_->selectionModel();
    if (selected_item->hasSelection())
    {
        QModelIndexList item_list = selected_item->selectedRows();
        if (item_list.size() > 0)
        {
            // Get the model index of the name column since column 0 (compare ID) is hidden and scrollTo
            // doesn't appear to scroll on hidden columns.
            QModelIndex model_index = model_->GetProxyModel()->index(item_list[0].row(), rra::kTlasInstancesColumnInstanceAddress);
            ui_->instances_table_->scrollTo(model_index, QAbstractItemView::ScrollHint::PositionAtTop);
            return;
        }
    }
    ui_->instances_table_->scrollToTop();
}