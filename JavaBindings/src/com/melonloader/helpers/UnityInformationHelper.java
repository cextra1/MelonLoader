package com.melonloader.helpers;

import android.app.Activity;
import android.content.Context;

import android.content.res.AssetManager;
import com.melonloader.ApplicationState;
import com.melonloader.Bootstrap;
import com.melonloader.LogBridge;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class UnityInformationHelper
{
    // TODO: add a way to handle APKs without globalgamemanagers
    // MelonLoader uses data.unity3d and mainData for PC,
    // but making copies of those would use a lot of storage I believe
    public static void SaveGlobalGameManagersToFile()
    {
        AssetManager am = ApplicationState.Context.getAssets();
        String unityInfoPath = ApplicationState.BaseDirectory + "/UnityInformation";
        File unityInformationFolder = new File(unityInfoPath);
        unityInformationFolder.mkdir();

        try
        {
            InputStream in = am.open("bin/Data/globalgamemanagers", 3);
            String newFileName = unityInformationFolder + "/globalgamemanagers";
            FileOutputStream out = new FileOutputStream(newFileName);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
        }
        catch (IOException ex)
        {
            LogBridge.error(ex.getMessage());
        }
    }
}
