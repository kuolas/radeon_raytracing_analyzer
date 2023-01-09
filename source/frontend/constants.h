//=============================================================================
// Copyright (c) 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief  Intended to hold globally-known definitions.
//=============================================================================

#ifndef RRA_CONSTANTS_H_
#define RRA_CONSTANTS_H_

#include <QColor>
#include <QUrl>

namespace rra
{
    static const QString kExecutableBaseFilename = "RadeonRaytracingAnalyzer";

#ifdef _DEBUG
    static const QString kExecutableDebugIdentifier = "-d";
#endif

    // @brief Checking for updates.
    static const QString kUpdateCheckAssetName          = "RRA-Updates.json";
    static const QString kUpdateCheckCheckingForUpdates = "Checking for updates...";
    static const QString kUpdateCheckNoUpdatesAvailable = "Checking for updates... done.\n\nNo updates available.";

#ifdef _DEBUG
    static const QString kUpdateCheckUrl = ".";
#else
    static const QString kUpdateCheckUrl = "https://api.github.com/repos/GPUOpen-Tools/radeon_raytracing_analyzer/releases/latest";
#endif

    static const int kUpdatesPendingDialogWidth  = 400;
    static const int kUpdatesPendingDialogHeight = 150;
    static const int kUpdatesResultsDialogWidth  = 400;
    static const int kUpdatesResultsDialogHeight = 300;

    /// @brief Control window sizes.
    static const int   kDesktopMargin                      = 25;
    static const float kDesktopAvailableWidthPercentage    = 99.0F;
    static const float kDesktopAvailableHeightPercentage   = 95.0F;
    static const float kDebugWindowDesktopWidthPercentage  = 66.0F;
    static const float kDebugWindowDesktopHeightPercentage = 25.0F;

    // @brief App colors.
    static const QColor kCheckboxEnableColor   = QColor(0, 122, 217);
    static const QColor kTableRowSelectedColor = QColor(204, 232, 255);

    /// @brief Control print formatting.
    static const int  kQtTooltipFloatPrecision = 7;    ///< Decimal place precision for tooltip of printed floating point values.
    static const char kQtFloatFormat           = 'f';  ///< Floating point format specifier.

    namespace text
    {
        // @brief Delete recent file pop up dialog.
        static const QString kDeleteRecentTraceTitle      = "Error";
        static const QString kDeleteRecentTraceTextFailed = "The trace failed to load.\n";
        static const QString kDeleteRecentTraceTextNoAS =
            "\nThere are no acceleration structures present in the trace file. "
            "Please ensure that the application being traced has ray tracing enabled "
            "and ray tracing is taking place at the time of capture. "
            "Also ensure that acceleration structures are not destroyed, cleared, or "
            "reused in the same frame they are created in.\n";
        static const QString kDeleteRecentTraceTextMalformedData =
            "\nThe trace file contains malformed data. If RRA captures are required, "
            "please ensure that raytracing buffers are not reused immediately after "
            "a DispatchRays call and defer reusing buffers until after a Present call.\n";

        static const QString kRemoveRecentTraceText = "\nThe trace is in the recent files list. Would you like to remove it?";

        // @brief Message box text displayed to a user when the viewport renderer fails to initialize.
        static const QString kRendererInitializationFailedTitle = "Renderer failure";

        // @brief Open recent trace missing pop up dialog.
        static const QString kOpenRecentTraceTitle = "Trace not opened";
        static const QString kOpenRecentTraceStart = "Trace \"";
        static const QString kOpenRecentTraceEnd   = "\"  does not exist!";

        // @brief The file extension for RRA trace files, and generic RDF files.
        static const QString kRRATraceFileExtension = ".rra";

        // @brief Help file locations.
        static const QString kTraceHelpFile       = "/docs/help/rdp/html/index.html";
        static const QString kHelpFile            = "/docs/help/rra/html/index.html";
        static const QString kLicenseFile         = "/License.txt";
        static const QString kSampleTraceLocation = "/samples/sample_trace" + kRRATraceFileExtension;
        static const QString kFileOpenFileTypes = "RRA trace files (*" + kRRATraceFileExtension + ")";
        static const QString kMissingHelpFile = "Missing RRA help file: ";

        // @brief External links.
        static const QUrl kGpuOpenUrl                = QUrl("https://gpuopen.com");
        static const QUrl kRraGithubUrl              = QUrl("https://github.com/GPUOpen-Tools/radeon_raytracing_analyzer");
        static const QUrl kRgpGpuOpenUrl             = QUrl("https://gpuopen.com/rgp/");
        static const QUrl kRgaGpuOpenUrl             = QUrl("https://gpuopen.com/rga/");
        static const QUrl kRdnaPerformanceGpuOpenUrl = QUrl("https://gpuopen.com/performance/");

        // @brief Treeview node ID selection types in the settings.
        static const QString kSettingsTreeviewOffset  = "Offset";
        static const QString kSettingsTreeviewAddress = "Virtual Address";

        /// @brief Event treeview expand/collapse button text.
        static const QString kTextExpandTree   = "Expand tree";    ///< Text string used for the button that expands a tree view.
        static const QString kTextCollapseTree = "Collapse tree";  ///< Text string used for the button that collapses a tree view.

        /// @brief Delimiter for strings that are split into arbitrary number of tokens.
        static const std::string kDelimiter = " ";

        /// @brief Delimiter for strings that are split into two tokens.
        static const std::string kDelimiterBinary = ":";

    }  // namespace text

    namespace resource
    {
        // @brief Stylesheet resource.
        static const QString kStylesheet = ":/Resources/stylesheet.qss";

    }  // namespace resource
}  // namespace rra

#endif  // RRA_CONSTANTS_H_
