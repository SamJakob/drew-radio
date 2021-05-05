#ifndef DR_STORAGE_FILE
#define DR_STORAGE_FILE

namespace DrewRadio {

    /**
     * Holds data from the file-backed data store in memory and provides
     * a convenient way to open and save/update that data in storage.
     */
    class FileStorage {

        SavedDevice** savedDevices;

        private:
            FileStorage();

    }

}

#endif
