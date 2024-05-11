package layaair.game.browser.Picture.bean;

import java.util.ArrayList;

public class CallbackRes {

    private ArrayList<String> tempFilePaths;
    private ArrayList<TempFiles> tempFiles;

    public void setTempFilePaths(ArrayList<String> tempFilePaths) {
        this.tempFilePaths = tempFilePaths;
    }

    public ArrayList<String> getTempFilePaths() {
        return tempFilePaths;
    }

    public void setTempFiles(ArrayList<TempFiles> tempFiles) {
        this.tempFiles = tempFiles;
    }

    public ArrayList<TempFiles> getTempFiles() {
        return tempFiles;
    }

    public static class TempFiles {

        private String path;
        private long size;

        public void setPath(String path) {
            this.path = path;
        }

        public String getPath() {
            return path;
        }

        public long getSize() {
            return size;
        }

        public void setSize(long size) {
            this.size = size;
        }
    }
}
