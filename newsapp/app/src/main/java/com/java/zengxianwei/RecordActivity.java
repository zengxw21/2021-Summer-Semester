package com.java.zengxianwei;

import android.content.Intent;
import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;

import com.google.android.material.appbar.MaterialToolbar;
import com.java.zengxianwei.api.ApiResponse;
import com.java.zengxianwei.api.ApiSingleNews;
import com.java.zengxianwei.api.ApiSingleNewsDao;
import com.java.zengxianwei.api.ApiSingleNewsDatabase;
import com.jcodecraeer.xrecyclerview.XRecyclerView;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;

public class RecordActivity extends AppCompatActivity {
    private ApiResponse mApiResponse;
    private XRecyclerView mXRecyclerView;
    private WeakReference<ApiSingleNewsDatabase> mDb;
    private ApiSingleNewsDao mApiSingleNewsDao;
    private Intent mIntent;
    private boolean isHistory;
    private NewsListRecyclerAdapter mNewsListRecyclerAdapter;
    private MaterialToolbar mToolBarView;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record);
        mXRecyclerView = findViewById(R.id.record_activity_recycler_view);
        mToolBarView = findViewById(R.id.record_activity_tool_bar);
        mApiResponse = new ApiResponse();
        mDb = new WeakReference<ApiSingleNewsDatabase>(MainActivity.getInstance().getDb());
        mApiSingleNewsDao = mDb.get().mApiSingleNewsDao();
        mIntent = getIntent();
        isHistory = mIntent.getBooleanExtra("isHistory",true);
        List<ApiSingleNews> newsList;
        if(isHistory){
            newsList = mApiSingleNewsDao.loadByRead();
            mToolBarView.setTitle("浏览历史");
        }
        else{
            newsList = mApiSingleNewsDao.loadByFavorite();
            mToolBarView.setTitle("我的收藏");
        }
        ArrayList<ApiSingleNews> newsArrayList = new ArrayList<ApiSingleNews>(newsList);
        mApiResponse.mPageSize=((Integer)newsArrayList.size()).toString();
        mApiResponse.mNewsList = newsArrayList;
        mNewsListRecyclerAdapter = new NewsListRecyclerAdapter(mApiResponse);
        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        mXRecyclerView.setLayoutManager(layoutManager);
        mXRecyclerView.setAdapter(mNewsListRecyclerAdapter);
        mXRecyclerView.setLoadingMoreEnabled(false);
        mXRecyclerView.setPullRefreshEnabled(false);
    }
    @Override
    protected void onResume() {
        super.onResume();
        List<ApiSingleNews> newsList;
        if (isHistory) {
            newsList = mApiSingleNewsDao.loadByRead();
        } else {
            newsList = mApiSingleNewsDao.loadByFavorite();
        }
        ArrayList<ApiSingleNews> newsArrayList=new ArrayList<ApiSingleNews>(newsList);
        mApiResponse.mPageSize=((Integer)newsArrayList.size()).toString();
        mApiResponse.mNewsList=newsArrayList;
        mNewsListRecyclerAdapter.setData(mApiResponse);
    }
    @Override
    public void onBackPressed() {
        super.onBackPressed();
        this.finish();
    }
}