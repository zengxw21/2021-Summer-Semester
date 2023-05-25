package com.java.zengxianwei.api;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class ApiKeyword {
    @SerializedName("score")
    @Expose
    private String mScore;
    @SerializedName("word")
    @Expose
    private String mKeyword;
    public String getScore(){
        return mScore;
    }
    public String getKeyword() {
        return mKeyword;
    }
}