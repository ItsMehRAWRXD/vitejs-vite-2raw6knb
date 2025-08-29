#ifndef DIGGZ_CONFIG_H
#define DIGGZ_CONFIG_H

#include <string>
#include <android/asset_manager.h>

class DiggzConfigLoader {
private:
    std::string app_data_dir;
    AAssetManager* asset_manager;
    
    bool extractAsset(const std::string& asset_path, const std::string& dest_path);
    bool createDirectory(const std::string& path);
    void writeConfig(const std::string& path, const std::string& content);
    bool extractZip(const std::string& zip_path, const std::string& dest_path);
    
    // Configuration methods
    void setupSeren();
    void setupTheCrew();
    void setupFen();
    void setupVenom();
    void setupTheOath();
    void setupSportsAddons();
    void setupTMDbHelper();
    void setupDebridConfig();
    void createFavoritesMenu();
    void setupRepositories();
    
public:
    DiggzConfigLoader(AAssetManager* assets, const std::string& data_dir);
    
    bool extractDiggzAssets();
    bool setupDiggzUserdata();
    bool configureDiggzAddons();
};

#endif // DIGGZ_CONFIG_H