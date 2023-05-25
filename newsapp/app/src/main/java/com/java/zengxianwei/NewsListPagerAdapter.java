package com.java.zengxianwei;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.lifecycle.Lifecycle;
import androidx.viewpager2.adapter.FragmentStateAdapter;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

public class NewsListPagerAdapter extends FragmentStateAdapter {
    ArrayList<String> mCategories;

    public void setData(ArrayList<String> data){
        mCategories = data;
        notifyDataSetChanged();
    }

    public NewsListPagerAdapter(@NonNull FragmentManager fm, Lifecycle lc,ArrayList<String> categories){
        super(fm,lc);
        mCategories = categories;
    }

    @Override
    public long getItemId(int position) {
        int i = 0;
        for(String cat:MainActivity.categories){
            if(mCategories.get(position).equals(cat)){
                return i;
            }
            i ++;
        }
        return 0;
    }

    @NonNull
    @Override
    public Fragment createFragment(int position) {

        Date now = new Date();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        String date = sdf.format(now);

        Fragment fragment = new NewsListItemFragment();
        Bundle args = new Bundle();
        args.putString("category",mCategories.get(position));
        args.putString("startTime","");
        args.putString("endTime",date);
        args.putString("keyword","");
        args.putInt("baseSize",50);
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public int getItemCount() {
        return mCategories.size();
    }
}