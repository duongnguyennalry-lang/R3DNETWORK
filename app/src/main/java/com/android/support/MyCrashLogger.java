package com.android.support;

import android.content.Context;
import android.os.Build;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

/**
 * Catches uncaught Java exceptions and writes them to a log file
 * under the app's external files directory (no extra permission needed).
 *
 * NOTE: this only catches JAVA exceptions. Native (C++/JNI) crashes such as
 * SIGSEGV will NOT be caught here - use `adb logcat` (look for "Fatal signal"
 * or "F DEBUG") to see those.
 */
public class MyCrashLogger implements Thread.UncaughtExceptionHandler {

    private final Thread.UncaughtExceptionHandler defaultHandler;
    private final Context appContext;

    private MyCrashLogger(Context context) {
        this.appContext = context.getApplicationContext();
        this.defaultHandler = Thread.getDefaultUncaughtExceptionHandler();
    }

    /** Call this once, as early as possible (Application.onCreate()). */
    public static void install(Context context) {
        Thread.setDefaultUncaughtExceptionHandler(new MyCrashLogger(context));
    }

    @Override
    public void uncaughtException(Thread thread, Throwable throwable) {
        try {
            writeCrashLog(throwable);
        } catch (Throwable ignored) {
            // never let the crash handler itself crash
        }
        if (defaultHandler != null) {
            defaultHandler.uncaughtException(thread, throwable);
        } else {
            System.exit(1);
        }
    }

    private void writeCrashLog(Throwable throwable) {
        File dir = appContext.getExternalFilesDir(null);
        if (dir == null) {
            dir = appContext.getFilesDir();
        }
        File crashDir = new File(dir, "crash_logs");
        if (!crashDir.exists()) {
            crashDir.mkdirs();
        }

        String timestamp = new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss", Locale.US).format(new Date());
        File logFile = new File(crashDir, "crash_" + timestamp + ".txt");

        StringWriter sw = new StringWriter();
        throwable.printStackTrace(new PrintWriter(sw));

        try (FileWriter writer = new FileWriter(logFile)) {
            writer.write("Time: " + timestamp + "\n");
            writer.write("Device: " + Build.MANUFACTURER + " " + Build.MODEL + "\n");
            writer.write("Android: " + Build.VERSION.RELEASE + " (SDK " + Build.VERSION.SDK_INT + ")\n");
            writer.write("ABI: " + Build.SUPPORTED_ABIS[0] + "\n\n");
            writer.write(sw.toString());
        } catch (Exception ignored) {
        }
    }
}
