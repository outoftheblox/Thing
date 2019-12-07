#include "FS.h"

void setup() {
    Serial.begin(115200);

    delay(5000);
    for (int i=1; i<=10; i++){
        Serial.println(millis());
        delay(1000);
    }



    SPIFFS.begin();
    FSInfo fs_info;
    SPIFFS.info(fs_info);

    float fileTotalKB = (float)fs_info.totalBytes / 1024.0;
    float fileUsedKB = (float)fs_info.usedBytes / 1024.0;

    float flashChipSize = (float)ESP.getFlashChipSize() / 1024.0 / 1024.0;
    float realFlashChipSize = (float)ESP.getFlashChipRealSize() / 1024.0 / 1024.0;
    float flashFreq = (float)ESP.getFlashChipSpeed() / 1000.0 / 1000.0;
    FlashMode_t ideMode = ESP.getFlashChipMode();

    Serial.println("==========================================================");
    Serial.println("Firmware: ");
    Serial.printf(" Chip Id: %08X\n", ESP.getChipId());
    Serial.print(" Core version: "); Serial.println(ESP.getCoreVersion());
    Serial.print(" SDK version: "); Serial.println(ESP.getSdkVersion());
    Serial.print(" Boot version: "); Serial.println(ESP.getBootVersion());
    Serial.print(" Boot mode: "); Serial.println(ESP.getBootMode());
    Serial.printf("__________________________\n\n");

    Serial.println("Flash chip information: ");
    Serial.printf(" Flash chip Id: %08X (for example: Id=001640E0 Manuf=E0, Device=4016 (swap bytes))\n", ESP.getFlashChipId());
    Serial.printf(" Sketch thinks Flash RAM is size: "); Serial.print(flashChipSize); Serial.println(" MB");
    Serial.print(" Actual size based on chip Id: "); Serial.print(realFlashChipSize); Serial.println(" MB ... given by (2^( \"Device\" - 1) / 8 / 1024");
    Serial.print(" Flash frequency: "); Serial.print(flashFreq); Serial.println(" MHz");
    Serial.printf(" Flash write mode: %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
    Serial.printf(" CPU frequency: %u MHz\n\n", ESP.getCpuFreqMHz());
    Serial.printf("__________________________\n\n");

    Serial.println("File system (SPIFFS): ");
    Serial.print(" Total KB: "); Serial.print(fileTotalKB); Serial.println(" KB");
    Serial.print(" Used KB: "); Serial.print(fileUsedKB); Serial.println(" KB");
    Serial.printf(" Block size: %lu\n", fs_info.blockSize);
    Serial.printf(" Page size: %lu\n", fs_info.pageSize);
    Serial.printf(" Maximum open files: %lu\n", fs_info.maxOpenFiles);
    Serial.printf(" Maximum path length: %lu\n\n", fs_info.maxPathLength);
    Serial.printf("__________________________\n\n");

    Dir dir = SPIFFS.openDir("/");
    Serial.println("SPIFFS directory {/} :");
    bool filesExist=false;
    while (dir.next()) {
        filesExist=true;
        Serial.print("File Name "); Serial.println(dir.fileName());
        Serial.print("File Length "); Serial.println(dir.fileSize());
    }
    if (!filesExist) {
        Serial.println("No files in SPIFFS");
    }

    File f = SPIFFS.open("/formatComplete.txt", "w");
    f.println("Format Complete");
    f.close();
    delay(200);

    if (!SPIFFS.exists("/formatComplete.txt")) {
        Serial.println("Please wait a few seconds for SPIFFS to be formatted");
        SPIFFS.format();
        delay(3000);
        //Serial.println("Spiffs formatted");
    }
    f = SPIFFS.open("/formatComplete.txt", "w");
    f.println("Format Complete");
    f.close();
    delay(200);

    f = SPIFFS.open("/formatComplete.txt", "r");
    if (!f) {
        Serial.println("bugger... format failed.");
    }
    else {

        String s=f.readStringUntil('\n');
        Serial.print("First line of file : ");
        Serial.println(s);

        Serial.println("SPIFFS is formatted. Moving along...");
        f.close();
    }

    dir = SPIFFS.openDir("/");
    Serial.println("SPIFFS directory {/} :");
    filesExist=false;
    while (dir.next()) {
        filesExist=true;
        Serial.print("File Name "); Serial.println(dir.fileName());
        Serial.print("File Length "); Serial.println(dir.fileSize());
    }
    if (!filesExist) {
        Serial.println("No files in SPIFFS");
    }

    while (dir.next()) {
        filesExist=true;
        SPIFFS.remove(dir.fileName());
    }
    dir = SPIFFS.openDir("/");
    Serial.println("SPIFFS directory {/} :");
    filesExist=false;
    while (dir.next()) {
        filesExist=true;
        Serial.print("File Name "); Serial.println(dir.fileName());
        Serial.print("File Length "); Serial.println(dir.fileSize());
    }
    if (!filesExist) {
        Serial.println("No files in SPIFFS");
    }
    delay(3000);
}

void loop() {
    // open file for writing
    File f1 = SPIFFS.open("/f1.txt", "w");
    if (!f1) {
        Serial.println("file open failed");
    }
    Serial.println("====== Writing to SPIFFS file =========");
    // write 10 strings to file
    for (int i=1; i<=10; i++){
        f1.print("Millis() : ");
        f1.println(millis());
        Serial.println(millis());
    }
    f1.close();

    // open file for reading
    f1 = SPIFFS.open("/f1.txt", "r");
    if (!f1) {
        Serial.println("file open failed");
    }

    Serial.println("====== Reading from SPIFFS file =======");
    // read 10 strings from file
    for (int i=1; i<=10; i++){
        String s=f1.readStringUntil('\n');
        Serial.print(i);
        Serial.print(":");
        Serial.println(s);
    }
    // wait a few seconds before doing it all over again
    delay(2000);

}
