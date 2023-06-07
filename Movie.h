#ifndef MOVIE_H
#define MOVIE_H

#include "wet1util.h"

/*
* Class Movie
* This class is used to represent a movie in the streaming system.
*/
class Movie {
public:

    /*
    * Constructor of Movie class
    * @param name - The movie's ID, genre, views, and whether the movie is a VIP movie
    * @return - A new instance of Movie
    */
    Movie(const int movieID, const Genre genre, const int views, const bool isVIP);

    /*
    * Copy Constructor and Assignment Operator of Movie class
    * streaming does not allow two of the same movie (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    * Additionally, we ensure that internal fields are not initialized as garbage values by deleting the default
    *   constructor.
    */
    Movie() = delete;
    Movie(const Movie& other) = delete;
    Movie& operator=(const Movie& other) = delete;

    /*
    * Destructor of Movie class
    * @param - none
    * @return - void
    */
    ~Movie() = default;

    /*
    * Return the movie's ID
    * @return - integer of the movie's ID
    */
    int get_Id() const;

    /*
    * Return whether the movie is a VIP
    * @return - bool - true if the movie is a VIP, otherwise false
    */
    bool isVIP() const;

    /*
    * Return the movie's genre
    * @return - Genre
    */
    Genre get_genre() const;

    /*
    * Return the movie's rating
    * @return - Genre
    */
    double get_rating() const;

    /*
    * Return the movie's views
    * @return - Genre
    */
    int get_views() const;

    /*
    * Increase the movie's views
    * @param - int, number of views to increase by (default = 1)
    * @return - void
    */
    void add_view(const int viewers = 1);

    /*
    * Add a rating to the movie's stats
    * @param - rating to be added
    * @return - void
    */
    void add_rating(const int rating);


private:

    /*
     * The internal fields of Movie:
     *   The movie's ID
     *   The movie's genre
     *   The number of times the movie was viewed
     *   Whether or not the movie is a VIP movie
     *   The number of people who rated the movie
     *   The movie's rating
     */   
   int m_movieID;
   const Genre m_genre;
   int m_views;
   bool m_VIP;
   int m_numRates;
   int m_rating;
};
#endif //MOVIE_H