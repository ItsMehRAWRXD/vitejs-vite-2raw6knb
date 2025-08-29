package com.mediaplayer.advanced;

import android.app.Activity;

/**
 * Native engine interface for Advanced Media Player
 * Handles all C++ interactions and Knox evasion
 */
public class NativeEngine {
    
    static {
        System.loadLibrary("advancedmediaplayer");
    }
    
    /**
     * Initialize the native engine
     * @param activity Current activity context
     */
    public native void initialize(Activity activity);
    
    /**
     * Check if Knox is active and potentially blocking
     * @return true if Knox is safe/inactive, false if Knox is active
     */
    public native boolean checkKnoxStatus();
    
    /**
     * Load the Diggz build configuration
     * Extracts and sets up all add-ons and configurations
     */
    public native void loadDiggzBuild();
    
    /**
     * Start the Kodi media center
     * @return 0 on success, negative on failure
     */
    public native int startKodi();
    
    /**
     * Apply Knox evasion techniques
     * Called when Knox is detected
     */
    public native void hideFromKnox();
}