package com.java.zengxianwei.api;

import androidx.annotation.NonNull;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;

public class ApiResponse {
    @SerializedName("pageSize")
    @Expose
    public String mPageSize;
    @SerializedName("total")
    @Expose
    private String mTotalNewsCount;
    @SerializedName("data")
    @Expose
    public ArrayList<com.java.zengxianwei.api.ApiSingleNews> mNewsList;

    public ArrayList<ApiSingleNews> getNewsList() {
        return mNewsList;
    }

    public int getPageSize() {
        if(mPageSize==null)
            return 0;
        return Integer.parseInt(mPageSize);
    }

    public int getTotalNewsCount() {
        if(mTotalNewsCount==null)
            return 0;
        return Integer.parseInt(mTotalNewsCount);
    }
    public int getNewsCount(){
        if(mNewsList==null)
            return 0;
        return mNewsList.size();
    }

    @NonNull
    @Override
    public String toString(){
        return mNewsList.toString();
    }
}
