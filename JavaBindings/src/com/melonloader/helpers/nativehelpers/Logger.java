package com.melonloader.helpers.nativehelpers;

import com.melonloader.ApplicationState;
import com.melonloader.Core;
import com.melonloader.LogBridge;
import com.melonloader.bridge.Assertion;

import java.io.File;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Logger
{
    static final String FilePrefix = "MelonLoader_";
    static final String FileExtension = ".log";
    static final String LatestLogFileName = "Latest";

    public static String Initialize()
    {
        File logFolderPath = Paths.get(ApplicationState.BaseDirectory, "melonloader", "etc", "logs").toFile();
        if (!logFolderPath.exists())
        {
            if (!logFolderPath.mkdir())
                Assertion.ThrowInternalFailure("Failed to Create Logs Folder!");
        }
        // TODO: CleanOldLogs()
        // else
        //   CleanOldLogs(logFolderPath);

        // Doing this here to make it easy
        File melonloaderPath = new File(logFolderPath.getParent() + "/" + LatestLogFileName + FileExtension);
        if (melonloaderPath.exists())
            melonloaderPath.delete();

        /*LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter f = DateTimeFormatter.ofPattern("yy-MM-dd_kk-mm-ss.SSS");
        String timeStamp = f.format(now);

        String filepath = Paths.get(logFolderPath.toString(), FilePrefix + timeStamp + FileExtension).toString();
        LogBridge.msg("[JavaBindings] Current log path at: " + filepath);*/
        return logFolderPath.getParent();
    }
}
