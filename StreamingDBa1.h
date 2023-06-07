// 
// 234218 Data Structures 1.
// Semester: 2023B (spring).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef STREAMINGDBA1_H_
#define STREAMINGDBA1_H_

#include "wet1util.h"
#include "User.h"
#include "Movie.h"
#include "Group.h"
#include "AVLTree.h"
#include "GenericNode.h"
#include "AVLMultiVariable.h"

class streaming_database {
private:

	int m_totalMovies;

	Tree<GenericNode<Movie*>, Movie*> m_moviesByID;
	Tree<GenericNode<User*>, User*> m_usersByID;
	Tree<GenericNode<Group*>, Group*> m_groupsByID;

	MultiTree<Movie*> m_moviesByRating;
	MultiTree<Movie*> m_comedyByRating;
	MultiTree<Movie*> m_dramaByRating;
	MultiTree<Movie*> m_actionByRating;
	MultiTree<Movie*> m_fantasyByRating;

    /*
     * helper function
     * Remove a movie and then insert it again in the right place
    * @param - genre of the movie, pointer to the movie, id of the movie, number of old views, old rating
    * @return - void
    */
    void insert_and_remove(const Genre genre, Movie* movie, const int movieId, const int oldViews, const double oldRating);

    /*
    * Helper function
    * @param - pointer to the movie, id of the movie
    * @return - void
    */
	void remove_by_genre(Movie* movie, const int movieId);
public:
	// <DO-NOT-MODIFY> {
	
	streaming_database();

	virtual ~streaming_database();
	
	StatusType add_movie(int movieId, Genre genre, int views, bool vipOnly);
	
	StatusType remove_movie(int movieId);
	
	StatusType add_user(int userId, bool isVip);
	
	StatusType remove_user(int userId);
	
	StatusType add_group(int groupId);
	
	StatusType remove_group(int groupId);

	StatusType add_user_to_group(int userId, int groupId);
	
	StatusType user_watch(int userId, int movieId);

    StatusType group_watch(int groupId,int movieId);

    output_t<int> get_all_movies_count(Genre genre);

    StatusType get_all_movies(Genre genre, int *const output);
	
	output_t<int> get_num_views(int userId, Genre genre);

    StatusType rate_movie(int userId, int movieId, int rating);
	
	output_t<int> get_group_recommendation(int groupId);
	
	// } </DO-NOT-MODIFY>
};

#endif // STREAMINGDBA1_H_
