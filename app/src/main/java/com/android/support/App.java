package com.android.support;

import android.app.Application;

public class App extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        MyCrashLogger.install(this);
    }
}
