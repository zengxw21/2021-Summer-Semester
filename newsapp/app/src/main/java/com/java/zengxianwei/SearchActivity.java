package com.java.zengxianwei;

import android.app.DatePickerDialog;
import android.app.Dialog;
import android.os.Bundle;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.Button;
import android.widget.DatePicker;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SearchView;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.fragment.app.DialogFragment;

import com.google.android.material.textfield.TextInputLayout;

import java.util.Calendar;

public class SearchActivity extends AppCompatActivity {
    private Button mLateTimeView;
    private Button mEarlyTimeView;
    private TextInputLayout mDropdownView;
    public static final int START_DATE = 1;
    public static final int END_DATE = -1;
    public static final int UNSELECTED = 0;
    private int mIsSettingStartingDate = 0;
    private String mStartDateTime = "";
    private String mEndDateTime = "";
    private AutoCompleteTextView mAutotext;
    private SearchView mSearchView;
    private ConstraintLayout mRootView;
    public static class DatePickerFragment extends DialogFragment implements
            DatePickerDialog.OnDateSetListener{

        @NonNull
        @Override
        public Dialog onCreateDialog(@Nullable Bundle savedInstanceState) {
            final Calendar c = Calendar.getInstance();
            int year  = c.get(Calendar.YEAR);
            int month = c.get(Calendar.MONTH);
            int day = c.get(Calendar.DAY_OF_MONTH);
            return new DatePickerDialog(getActivity(),this,year,month,day);
        }

        public static String fromDateTimeString(int year,int month,int day){
            String res = ((Integer)year).toString()+"-";
            if(month<10){
                res+="0";
            }
            res+=((Integer)month).toString() + "-";
            if(day<10){
                res+="0";
            }
            res +=((Integer)day).toString();
            return res;
        }
        @Override
        public void onDateSet(DatePicker datePicker, int year, int month, int day) {
            month++;//????
            SearchActivity instance = (SearchActivity) getContext();
            if(instance.mIsSettingStartingDate == instance.START_DATE){
                String date = fromDateTimeString(year,month,day);
                instance.mEarlyTimeView.setText(date);
                instance.mStartDateTime = date;
            }
            if(instance.mIsSettingStartingDate == instance.END_DATE){
                String date = fromDateTimeString(year, month, day);
                instance.mLateTimeView.setText(date);
                instance.mEndDateTime = date;
            }
        }
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.search_list_activity);
        mDropdownView = findViewById(R.id.news_search_dropdown);
        mEarlyTimeView = findViewById(R.id.news_search_earliest_time);
        mLateTimeView = findViewById(R.id.news_search_latest_time);
        mAutotext = findViewById(R.id.news_search_AutocompleteText);
        mSearchView = findViewById(R.id.news_search_bar);
        mRootView = findViewById(R.id.search_view_root);
        mSearchView.setIconified(false);
        mEarlyTimeView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mIsSettingStartingDate = START_DATE;
                DialogFragment datePickerFragment = new DatePickerFragment();
                datePickerFragment.show(getSupportFragmentManager(),"earlyDatePicker");
            }
        });
        mLateTimeView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mIsSettingStartingDate = END_DATE;
                DialogFragment datePickerFragment = new DatePickerFragment();
                datePickerFragment.show(getSupportFragmentManager(),"lateDatePicker");
            }
        });
        String[] categories = {"任意","娱乐", "军事", "教育", "文化", "健康", "财经", "体育", "汽车", "科技", "社会"};
        ArrayAdapter<String> dropDownAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, categories);
        mAutotext.setAdapter(dropDownAdapter);
        //int SearchBarId = mSearchView.getContext().getResources().getIdentifier("android:id/search_src_text", null, null);
       /* View searchBar = mSearchView.findViewById(SearchBarId);
        ((EditText)searchBar).setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if(id == EditorInfo.IME_ACTION_SEARCH){
                    startSearch(textView.getText().toString());
                    return true;
                }
                return false;
            }
        });*/
        mSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                System.out.println(query);
                startSearch(query);
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                return false;
            }
        });
    }
    public void startSearch(String key){
        Bundle args = new Bundle();
        args.putString("category", mDropdownView.getEditText().getText().toString());
        args.putString("startTime",mStartDateTime);
        args.putString("endTime",mEndDateTime);
        args.putString("keyword",key);
        args.putInt("baseSize",50);
        /* System.out.println(mStartDateTime); */
        getSupportFragmentManager().beginTransaction()
                .setReorderingAllowed(true)
                .add(R.id.news_search_frameholder, NewsListItemFragment.class, args)
                .commit();
        InputMethodManager imm = (InputMethodManager)getSystemService(this.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(getCurrentFocus().getWindowToken(), 0);
        return;
    }

    @Override
    protected void onResume() {
        super.onResume();
        mRootView.requestFocus();
    }
}