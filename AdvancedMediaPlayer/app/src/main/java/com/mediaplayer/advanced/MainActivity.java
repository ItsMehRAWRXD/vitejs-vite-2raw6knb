package com.mediaplayer.advanced;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends Activity {
    private static final String TAG = "AdvancedMedia";
    
    private NativeEngine nativeEngine;
    private Handler mainHandler;
    private ProgressBar progressBar;
    private TextView statusText;
    private Button startButton;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        Log.i(TAG, "Advanced Media Player starting...");
        
        // Initialize UI
        initializeUI();
        
        // Initialize native engine
        nativeEngine = new NativeEngine();
        mainHandler = new Handler(Looper.getMainLooper());
        
        // Initialize native components
        nativeEngine.initialize(this);
        
        // Delayed startup to avoid Knox detection
        mainHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                checkSystemAndInitialize();
            }
        }, 2000); // 2 second delay
    }
    
    private void initializeUI() {
        progressBar = findViewById(R.id.progressBar);
        statusText = findViewById(R.id.statusText);
        startButton = findViewById(R.id.startButton);
        
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startMediaCenter();
            }
        });
        
        // Initially hide start button
        startButton.setVisibility(View.GONE);
    }
    
    private void checkSystemAndInitialize() {
        updateStatus("Checking system compatibility...");
        
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    // Check Knox status
                    boolean knoxSafe = nativeEngine.checkKnoxStatus();
                    
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            if (!knoxSafe) {
                                updateStatus("Applying security optimizations...");
                                nativeEngine.hideFromKnox();
                            }
                            
                            loadDiggzConfiguration();
                        }
                    });
                    
                } catch (Exception e) {
                    Log.e(TAG, "Error during system check", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            updateStatus("System check failed");
                            Toast.makeText(MainActivity.this, "Initialization failed", Toast.LENGTH_SHORT).show();
                        }
                    });
                }
            }
        }).start();
    }
    
    private void loadDiggzConfiguration() {
        updateStatus("Loading media configuration...");
        
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    // Load Diggz build
                    nativeEngine.loadDiggzBuild();
                    
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            updateStatus("Configuration loaded successfully");
                            progressBar.setVisibility(View.GONE);
                            startButton.setVisibility(View.VISIBLE);
                            startButton.setText("Start Media Center");
                        }
                    });
                    
                } catch (Exception e) {
                    Log.e(TAG, "Error loading configuration", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            updateStatus("Configuration loading failed");
                            Toast.makeText(MainActivity.this, "Configuration failed", Toast.LENGTH_SHORT).show();
                        }
                    });
                }
            }
        }).start();
    }
    
    private void startMediaCenter() {
        updateStatus("Starting media center...");
        startButton.setEnabled(false);
        
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    int result = nativeEngine.startKodi();
                    
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            if (result == 0) {
                                updateStatus("Media center started successfully");
                                // Hide the launcher activity after successful start
                                finish();
                            } else {
                                updateStatus("Failed to start media center");
                                startButton.setEnabled(true);
                                Toast.makeText(MainActivity.this, "Startup failed", Toast.LENGTH_SHORT).show();
                            }
                        }
                    });
                    
                } catch (Exception e) {
                    Log.e(TAG, "Error starting media center", e);
                    mainHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            updateStatus("Startup error");
                            startButton.setEnabled(true);
                            Toast.makeText(MainActivity.this, "Startup error", Toast.LENGTH_SHORT).show();
                        }
                    });
                }
            }
        }).start();
    }
    
    private void updateStatus(String status) {
        statusText.setText(status);
        Log.i(TAG, "Status: " + status);
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "MainActivity destroyed");
    }
}