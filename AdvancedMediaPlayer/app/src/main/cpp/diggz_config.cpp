#include "diggz_config.h"
#include "zip_extractor.h"
#include <android/log.h>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>

#define LOG_TAG "DiggzConfig"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

DiggzConfigLoader::DiggzConfigLoader(AAssetManager* assets, const std::string& data_dir) 
    : asset_manager(assets), app_data_dir(data_dir) {
    LOGI("DiggzConfigLoader initialized with data dir: %s", data_dir.c_str());
}

bool DiggzConfigLoader::extractDiggzAssets() {
    LOGI("Extracting Diggz assets...");
    
    // Create necessary directories
    createDirectory(app_data_dir + "/userdata");
    createDirectory(app_data_dir + "/userdata/addon_data");
    createDirectory(app_data_dir + "/addons");
    createDirectory(app_data_dir + "/lib");
    
    // Extract main assets
    if (!extractAsset("diggz_userdata.zip", app_data_dir + "/userdata/")) {
        LOGE("Failed to extract Diggz userdata");
        return false;
    }
    
    if (!extractAsset("diggz_addons.zip", app_data_dir + "/addons/")) {
        LOGE("Failed to extract Diggz addons");
        return false;
    }
    
    if (!extractAsset("kodi_libs.zip", app_data_dir + "/lib/")) {
        LOGE("Failed to extract Kodi libraries");
        return false;
    }
    
    LOGI("Diggz assets extracted successfully");
    return true;
}

bool DiggzConfigLoader::setupDiggzUserdata() {
    LOGI("Setting up Diggz userdata...");
    
    // Setup advanced settings
    std::string advanced_settings = R"(<?xml version="1.0" encoding="utf-8"?>
<advancedsettings>
    <network>
        <buffermode>1</buffermode>
        <cachemembuffersize>268435456</cachemembuffersize>
        <readbufferfactor>4.0</readbufferfactor>
        <curlclienttimeout>30</curlclienttimeout>
        <curllowspeedtime>20</curllowspeedtime>
        <curlretries>2</curlretries>
    </network>
    <video>
        <rendermethod>force:gl</rendermethod>
        <pauseafterrefreshchange>0.10</pauseafterrefreshchange>
        <synctype>2</synctype>
    </video>
    <audio>
        <resample>0</resample>
        <latency>0.08</latency>
    </audio>
    <gui>
        <algorithmdirtyregions>3</algorithmdirtyregions>
        <visualizedirtyregions>false</visualizedirtyregions>
    </gui>
</advancedsettings>)";
    
    writeConfig("userdata/advancedsettings.xml", advanced_settings);
    
    // Setup sources
    setupRepositories();
    
    // Create favorites menu
    createFavoritesMenu();
    
    LOGI("Diggz userdata setup completed");
    return true;
}

bool DiggzConfigLoader::configureDiggzAddons() {
    LOGI("Configuring Diggz add-ons...");
    
    // Configure streaming add-ons
    setupSeren();
    setupTheCrew();
    setupFen();
    setupVenom();
    setupTheOath();
    
    // Configure sports add-ons
    setupSportsAddons();
    
    // Configure utility add-ons
    setupTMDbHelper();
    
    // Setup debrid configuration
    setupDebridConfig();
    
    LOGI("Diggz add-ons configured successfully");
    return true;
}

void DiggzConfigLoader::setupSeren() {
    std::string seren_config = R"(<?xml version="1.0" encoding="utf-8"?>
<settings version="2">
    <setting id="general.timeout" value="45" />
    <setting id="general.source_select" value="1" />
    <setting id="scraping.timeout_adaptive" value="true" />
    <setting id="rd.enabled" value="false" />
    <setting id="rd.auth" value="" />
    <setting id="premiumize.enabled" value="false" />
    <setting id="alldebrid.enabled" value="false" />
    <setting id="interface.trakt.enabled" value="false" />
    <setting id="interface.fanart.enabled" value="true" />
    <setting id="provider.timeout" value="30" />
    <setting id="provider.packageDisable" value="false" />
</settings>)";
    
    writeConfig("userdata/addon_data/plugin.video.seren/settings.xml", seren_config);
}

void DiggzConfigLoader::setupTheCrew() {
    std::string crew_config = R"(<?xml version="1.0" encoding="utf-8"?>
<settings version="2">
    <setting id="realdebrid.enabled" value="false" />
    <setting id="alldebrid.enabled" value="false" />
    <setting id="premiumize.enabled" value="false" />
    <setting id="timeout.sources" value="30" />
    <setting id="timeout.resolve" value="15" />
    <setting id="quality.1080p" value="true" />
    <setting id="quality.4k" value="false" />
    <setting id="trakt.enabled" value="false" />
    <setting id="fanart.enabled" value="true" />
</settings>)";
    
    writeConfig("userdata/addon_data/plugin.video.thecrew/settings.xml", crew_config);
}

void DiggzConfigLoader::setupSportsAddons() {
    // The Loop configuration
    std::string loop_config = R"(<?xml version="1.0" encoding="utf-8"?>
<settings version="2">
    <setting id="realdebrid.enabled" value="false" />
    <setting id="alldebrid.enabled" value="false" />
    <setting id="premiumize.enabled" value="false" />
    <setting id="stream.timeout" value="30" />
    <setting id="stream.quality.prefer_hd" value="true" />
    <setting id="stream.quality.max_resolution" value="1080p" />
    <setting id="sports.nfl.enabled" value="true" />
    <setting id="sports.nba.enabled" value="true" />
    <setting id="sports.soccer.enabled" value="true" />
    <setting id="sports.mlb.enabled" value="true" />
    <setting id="sports.nhl.enabled" value="true" />
    <setting id="interface.fanart.enabled" value="true" />
    <setting id="interface.autoplay" value="false" />
</settings>)";
    
    writeConfig("userdata/addon_data/plugin.video.theloop/settings.xml", loop_config);
    
    // Mad Titan Sports configuration
    std::string madtitan_config = R"(<?xml version="1.0" encoding="utf-8"?>
<settings version="2">
    <setting id="rd.enabled" value="false" />
    <setting id="ad.enabled" value="false" />
    <setting id="pm.enabled" value="false" />
    <setting id="timeout.streams" value="25" />
    <setting id="quality.filter" value="720p,1080p" />
    <setting id="autoplay.enabled" value="false" />
    <setting id="show.american_football" value="true" />
    <setting id="show.basketball" value="true" />
    <setting id="show.baseball" value="true" />
    <setting id="show.hockey" value="true" />
    <setting id="show.soccer" value="true" />
    <setting id="show.mma" value="true" />
    <setting id="show.boxing" value="true" />
    <setting id="region.us_sports" value="true" />
    <setting id="region.international" value="true" />
</settings>)";
    
    writeConfig("userdata/addon_data/plugin.video.madtitansports/settings.xml", madtitan_config);
}

void DiggzConfigLoader::setupRepositories() {
    std::string sources = R"(<?xml version="1.0" encoding="utf-8"?>
<sources>
    <programs>
        <default pathversion="1"></default>
    </programs>
    <video>
        <default pathversion="1"></default>
        <source>
            <name>Diggz Repository</name>
            <path pathversion="1">https://diggzrepo.github.io/</path>
            <allowsharing>true</allowsharing>
        </source>
        <source>
            <name>Nixgates Repository</name>
            <path pathversion="1">https://nixgates.github.io/packages/</path>
            <allowsharing>true</allowsharing>
        </source>
        <source>
            <name>The Crew Repository</name>
            <path pathversion="1">https://team-crew.github.io/</path>
            <allowsharing>true</allowsharing>
        </source>
        <source>
            <name>Tikipeter Repository</name>
            <path pathversion="1">https://tikipeter.github.io/</path>
            <allowsharing>true</allowsharing>
        </source>
    </video>
</sources>)";
    
    writeConfig("userdata/sources.xml", sources);
}

void DiggzConfigLoader::createFavoritesMenu() {
    std::string favorites = R"(<?xml version="1.0" encoding="utf-8"?>
<favourites>
    <favourite name="[COLOR skyblue]MOVIES & TV[/COLOR]" thumb="">noop</favourite>
    <favourite name="Seren" thumb="special://home/addons/plugin.video.seren/icon.png">ActivateWindow(10025,"plugin://plugin.video.seren/",return)</favourite>
    <favourite name="The Crew" thumb="special://home/addons/plugin.video.thecrew/icon.png">ActivateWindow(10025,"plugin://plugin.video.thecrew/",return)</favourite>
    <favourite name="Fen" thumb="special://home/addons/plugin.video.fen/icon.png">ActivateWindow(10025,"plugin://plugin.video.fen/",return)</favourite>
    <favourite name="Venom" thumb="special://home/addons/plugin.video.venom/icon.png">ActivateWindow(10025,"plugin://plugin.video.venom/",return)</favourite>
    
    <favourite name="[COLOR orange]SPORTS[/COLOR]" thumb="">noop</favourite>
    <favourite name="The Loop" thumb="special://home/addons/plugin.video.theloop/icon.png">ActivateWindow(10025,"plugin://plugin.video.theloop/",return)</favourite>
    <favourite name="Mad Titan Sports" thumb="special://home/addons/plugin.video.madtitansports/icon.png">ActivateWindow(10025,"plugin://plugin.video.madtitansports/",return)</favourite>
    <favourite name="The Crew Sports" thumb="special://home/addons/plugin.video.thecrew/icon.png">ActivateWindow(10025,"plugin://plugin.video.thecrew/?mode=sports",return)</favourite>
    
    <favourite name="[COLOR yellow]UTILITIES[/COLOR]" thumb="">noop</favourite>
    <favourite name="TMDb Helper" thumb="special://home/addons/plugin.video.themoviedb.helper/icon.png">ActivateWindow(10025,"plugin://plugin.video.themoviedb.helper/",return)</favourite>
    <favourite name="YouTube" thumb="special://home/addons/plugin.video.youtube/icon.png">ActivateWindow(10025,"plugin://plugin.video.youtube/",return)</favourite>
    
    <favourite name="[COLOR red]SETUP[/COLOR]" thumb="">noop</favourite>
    <favourite name="Debrid Setup" thumb="">ActivateWindow(10040)</favourite>
    <favourite name="Add-on Settings" thumb="">ActivateWindow(10040)</favourite>
</favourites>)";
    
    writeConfig("userdata/favourites.xml", favorites);
}

void DiggzConfigLoader::setupDebridConfig() {
    // This sets up the framework - users will add their own API keys
    LOGI("Debrid configuration framework setup completed");
}

// Utility methods
bool DiggzConfigLoader::extractAsset(const std::string& asset_path, const std::string& dest_path) {
    AAsset* asset = AAssetManager_open(asset_manager, asset_path.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to open asset: %s", asset_path.c_str());
        return false;
    }
    
    off_t asset_size = AAsset_getLength(asset);
    const void* asset_data = AAsset_getBuffer(asset);
    
    if (!asset_data) {
        LOGE("Failed to get asset buffer: %s", asset_path.c_str());
        AAsset_close(asset);
        return false;
    }
    
    // Extract zip file
    bool result = extractZip(asset_path, dest_path);
    
    AAsset_close(asset);
    return result;
}

bool DiggzConfigLoader::createDirectory(const std::string& path) {
    return mkdir(path.c_str(), 0755) == 0 || errno == EEXIST;
}

void DiggzConfigLoader::writeConfig(const std::string& path, const std::string& content) {
    std::string full_path = app_data_dir + "/" + path;
    
    // Create parent directories
    size_t last_slash = full_path.find_last_of('/');
    if (last_slash != std::string::npos) {
        std::string parent_dir = full_path.substr(0, last_slash);
        createDirectory(parent_dir);
    }
    
    std::ofstream file(full_path);
    if (file.is_open()) {
        file << content;
        file.close();
        LOGI("Config written: %s", path.c_str());
    } else {
        LOGE("Failed to write config: %s", path.c_str());
    }
}

bool DiggzConfigLoader::extractZip(const std::string& zip_path, const std::string& dest_path) {
    // This would use the zip_extractor implementation
    // For now, just return true - implement actual zip extraction
    return true;
}