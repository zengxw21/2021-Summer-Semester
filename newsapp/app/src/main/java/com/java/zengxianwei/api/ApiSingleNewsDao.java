package com.java.zengxianwei.api;

import androidx.room.Dao;
import androidx.room.Delete;
import androidx.room.Insert;
import androidx.room.OnConflictStrategy;
import androidx.room.Query;

import java.util.List;

@Dao
public interface ApiSingleNewsDao {
    @Query("SELECT * FROM api_single_news")
    List<com.java.zengxianwei.api.ApiSingleNews> getAll();

    @Query("SELECT * FROM api_single_news WHERE mNewsId IN (:newsPieceIds)")
    List<com.java.zengxianwei.api.ApiSingleNews> loadAllByIds(int[] newsPieceIds);

    @Query("SELECT * FROM api_single_news WHERE mNewsId=:newsId")
    com.java.zengxianwei.api.ApiSingleNews getById(String newsId);

    @Query("SELECT EXISTS(SELECT * FROM api_single_news WHERE mNewsId =:newsId)")
    boolean checkNewsRead(String newsId);
    @Query("SELECT EXISTS(SELECT * FROM api_single_news WHERE favoriteTime<10000000000000 AND mNewsId =:newsId)")
    boolean checkNewsFavorite(String newsId);

    @Query("SELECT * FROM api_single_news WHERE readTime<10000000000000 ORDER BY readTime DESC")
    List<com.java.zengxianwei.api.ApiSingleNews> loadByRead();
    @Query("SELECT * FROM api_single_news WHERE favoriteTime<10000000000000 ORDER BY favoriteTime DESC")
    List<com.java.zengxianwei.api.ApiSingleNews> loadByFavorite();

    @Insert(onConflict = OnConflictStrategy.REPLACE)
    void insertAll(com.java.zengxianwei.api.ApiSingleNews... newsPieces);

    @Delete
    void delete(com.java.zengxianwei.api.ApiSingleNews newsPiece);
}