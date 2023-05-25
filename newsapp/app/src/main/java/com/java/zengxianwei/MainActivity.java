package com.java.zengxianwei;

import android.content.Intent;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.PopupWindow;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.cardview.widget.CardView;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;
import androidx.room.Room;
import androidx.viewpager2.widget.ViewPager2;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.tabs.TabLayout;
import com.google.android.material.tabs.TabLayoutMediator;
import com.java.zengxianwei.api.ApiRetrofitNews;
import com.java.zengxianwei.api.ApiSingleNewsDatabase;

import java.lang.reflect.Field;
import java.util.ArrayList;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "Mainactivity";
    public static final String[] categories = {"娱乐","军事","教育","文化","健康","财经","体育","汽车","科技","社会"};
    public static ArrayList<String> mCustomCategories = new ArrayList<String>();
    private static MainActivity mInstance = null;

    private ApiSingleNewsDatabase db = null;
    private NewsListPagerAdapter mNewsListPagerAdapter;
    private ViewPager2 mViewPager2;
    private TabLayout mTopicTabs;
    private Retrofit mRetrofit;
    private ApiRetrofitNews mApiRetrofitNews;
    private FloatingActionButton mFloatingSearchButton;
    private Toolbar mToolBar;
    private ImageView mPopupButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        mInstance = this;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mToolBar = (Toolbar) findViewById(R.id.main_activity_top_bar);
        setSupportActionBar(mToolBar);
        for(String cat : categories){
            mCustomCategories.add(cat);
        }
        mFloatingSearchButton = findViewById(R.id.floating_search_button);
        mPopupButton = findViewById(R.id.main_activity_popup_button);
        mFloatingSearchButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent gotosearch = new Intent(getInstance(),SearchActivity.class);
                getInstance().startActivity(gotosearch);
            }
        });
        mViewPager2 = findViewById(R.id.view_pager2);
        mNewsListPagerAdapter = new NewsListPagerAdapter(getSupportFragmentManager(),getLifecycle(),mCustomCategories);
        mViewPager2.setAdapter(mNewsListPagerAdapter);
        mTopicTabs = findViewById(R.id.tab_layout);
        new TabLayoutMediator(mTopicTabs,mViewPager2,(tab, position) -> tab.setText(mCustomCategories.get(position))).attach();
        mRetrofit = new Retrofit.Builder().baseUrl("https://api2.newsminer.net/").addConverterFactory(GsonConverterFactory.create()).build();
        mApiRetrofitNews = mRetrofit.create(ApiRetrofitNews.class);
        try {
            final Field recyclerViewField = ViewPager2.class.getDeclaredField("mTouchSlop");
            recyclerViewField.setAccessible(true);

            final RecyclerView recyclerView = (RecyclerView) recyclerViewField.get(mViewPager2);

            final Field touchSlopField = RecyclerView.class.getDeclaredField("mTouchSlop");
            touchSlopField.setAccessible(true);

            final int touchSlop = (int) touchSlopField.get(recyclerView);
            touchSlopField.set(recyclerView, touchSlop * 3);
        }catch (Exception ignore){
        }
        mPopupButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                LayoutInflater inflater = (LayoutInflater)
                        getSystemService(LAYOUT_INFLATER_SERVICE);
                View popupView = inflater.inflate(R.layout.popup_window,null);
                int width = dpToPx(230);
                int height = CardView.LayoutParams.WRAP_CONTENT;
                boolean focusable = true; // lets taps outside the popup also dismiss it
                final PopupWindow popupWindow = new PopupWindow(popupView, width, height, focusable);

                popupWindow.showAtLocation(findViewById(R.id.main_activity_root_layout), Gravity.RIGHT| Gravity.TOP, 0,400);

                popupView.setOnTouchListener(new View.OnTouchListener() {
                    @Override
                    public boolean onTouch(View view, MotionEvent motionEvent) {
                        popupWindow.dismiss();
                        return false;
                    }
                });
                LinearLayoutManager layoutManager = new LinearLayoutManager(view.getContext());
                layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
                PopupRecyclerAdapter mPopupRecyclerAdapter = new PopupRecyclerAdapter();
                RecyclerView recyclerView = popupView.findViewById(R.id.popup_recycler_view);
                recyclerView.setLayoutManager(layoutManager);
                recyclerView.setAdapter(mPopupRecyclerAdapter);
                ItemTouchHelper.Callback callback =
                        new PopupRecyclerAdapter.ItemTouchHelperCallbackClass(mPopupRecyclerAdapter);
                ItemTouchHelper touchHelper = new ItemTouchHelper(callback);
                touchHelper.attachToRecyclerView(recyclerView);
            }
        });
    }

    public static MainActivity getInstance(){
        return mInstance;
    }

    public void resetTabData(){
        mNewsListPagerAdapter.setData(mCustomCategories);
    }

    public ApiSingleNewsDatabase getDb(){
        if(db == null){
            db = Room.databaseBuilder(getApplicationContext(),ApiSingleNewsDatabase.class,"api_single_news").allowMainThreadQueries().fallbackToDestructiveMigration().build();
        }
        return db;
    }

    public int dpToPx(int dp){
        DisplayMetrics displayMetrics = this.getResources().getDisplayMetrics();
        return Math.round(dp * (displayMetrics.xdpi / DisplayMetrics.DENSITY_DEFAULT));

    }

    @Override
    public boolean onCreateOptionsMenu(@NonNull Menu menu) {
        getMenuInflater().inflate(R.menu.top_bar_buttons,menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_favorite:
                Intent gotoFavorite = new Intent(getInstance(), RecordActivity.class);
                gotoFavorite.putExtra("isHistory",false);
                getInstance().startActivity(gotoFavorite);
                return true;

            case R.id.action_search:
                Intent gotoSearch = new Intent(getInstance(), SearchActivity.class);
                getInstance().startActivity(gotoSearch);
                return true;
            case R.id.action_history:
                Intent gotoHistory = new Intent(getInstance(), RecordActivity.class);
                gotoHistory.putExtra("isHistory",true);
                getInstance().startActivity(gotoHistory);
                return true;

            default:
                return super.onOptionsItemSelected(item);

        }
    }
}