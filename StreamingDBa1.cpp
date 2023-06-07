#include "StreamingDBa1.h"

streaming_database::streaming_database():
    m_totalMovies(0),
    m_moviesByID(),
    m_usersByID(),
    m_groupsByID(),
    m_moviesByRating(),
    m_comedyByRating(),
    m_dramaByRating(),
    m_actionByRating(),
    m_fantasyByRating()
{}

streaming_database::~streaming_database()
{
	if (m_totalMovies > 0) {
		m_moviesByID.erase_data(m_moviesByID.m_node);
	}
	if (m_usersByID.m_node->get_height() >= 0) {
		m_usersByID.erase_data(m_usersByID.m_node);
	}
	if (m_groupsByID.m_node->get_height() >= 0) {
		m_groupsByID.erase_data(m_groupsByID.m_node);
	}
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly) {
    if (movieId <= 0 || genre == Genre::NONE || views < 0)
        return StatusType::INVALID_INPUT;
    Movie *newMovie;
    try {
        newMovie = new Movie(movieId, genre, views, vipOnly);
        m_moviesByID.insert(newMovie, movieId);
        m_moviesByRating.insert(newMovie, movieId, views, 0);
    }
    catch (const std::bad_alloc &e) {
        delete newMovie;
        return StatusType::ALLOCATION_ERROR;
    }
    catch (const InvalidID &e) {
        delete newMovie;
        return StatusType::FAILURE;
    }
    m_totalMovies++;
    try {
        switch (genre) {
            case Genre::COMEDY:
                m_comedyByRating.insert(newMovie, movieId, views, 0);
                break;
            case Genre::DRAMA:
                m_dramaByRating.insert(newMovie, movieId, views, 0);
                break;
            case Genre::ACTION:
                m_actionByRating.insert(newMovie, movieId, views, 0);
                break;
            case Genre::FANTASY:
                m_fantasyByRating.insert(newMovie, movieId, views, 0);
                break;
            default:
                break;
        }
    }
    catch (const std::bad_alloc &e) {
        m_moviesByID.remove(movieId);
        m_moviesByRating.remove(movieId, views, 0);
        delete newMovie;
        return StatusType::ALLOCATION_ERROR;
    }
    catch (const InvalidID& e) {
        m_moviesByID.remove(movieId);
        m_moviesByRating.remove(movieId, views, 0);
        delete newMovie;
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if (movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Movie* movie = m_moviesByID.search_and_return_data(movieId);
        remove_by_genre(movie, movieId);
        m_moviesByRating.remove(movieId, movie->get_views(), movie->get_rating());
        m_moviesByID.remove(movieId);
        m_totalMovies--;
        delete movie;
    }
    catch(const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    catch(const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
	if(userId<=0) {
        return StatusType::INVALID_INPUT;
    }
    User *newUser;
    try {
        newUser = new User(userId, isVip);
        m_usersByID.insert(newUser, userId);
    }
    catch (const std::bad_alloc &e) {
        delete newUser;
        return StatusType::ALLOCATION_ERROR;
    }
    catch(const InvalidID& e) {
        delete newUser;
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if (userId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        User* user = m_usersByID.search_and_return_data(userId);
        if (user->get_group() != nullptr) {
            StatusType status = (user->get_group())->remove_user(user, userId, user->isVIP());
            if (status != StatusType::SUCCESS) {
                return status;
            }
        }
        m_usersByID.remove(userId);
        delete user;
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if(groupId<=0) {
        return StatusType::INVALID_INPUT;
    }
    Group *newGroup;
    try {
        newGroup = new Group(groupId);
        m_groupsByID.insert(newGroup, groupId);
    }
    catch (const std::bad_alloc &e) {
        delete newGroup;
        return StatusType::ALLOCATION_ERROR;
    }
    catch (InvalidID &e) {
    delete newGroup;
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if (groupId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Group* group = m_groupsByID.search_and_return_data(groupId);
        group->remove_group();
        m_groupsByID.remove(groupId);
        delete group;
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    catch (const std::bad_alloc& e) {
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if(userId<=0 || groupId<=0) {
        return StatusType::INVALID_INPUT;
    }
    User* user;
    Group* group;
    try {
        user = m_usersByID.search_and_return_data(userId);
        group = m_groupsByID.search_and_return_data(groupId);
    }
    catch (const InvalidID &e) {
        return StatusType::FAILURE;
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    if(!user->get_group()) {
        group->add_user(user, userId, user->isVIP());
        user->update_group(group);
    }
    else {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    int oldViews = 0;
    Movie* movie  = nullptr;
    if (userId <= 0 || movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        User* user = m_usersByID.search_and_return_data(userId);
        movie = m_moviesByID.search_and_return_data(movieId);
        if (movie->isVIP()) {
            if (!(user->isVIP())) {
                return StatusType::FAILURE;
            }
        }
        oldViews = movie->get_views();
        movie->add_view();
        Genre genre = movie->get_genre();
        insert_and_remove(genre, movie, movieId, oldViews, movie->get_rating());
        user->add_view(genre);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    catch (const std::bad_alloc& e) {
        movie->add_view(-1);
        m_moviesByID.insert(movie, movieId);
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    Movie* movie;
    if (groupId <= 0 || movieId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Group* group = m_groupsByID.search_and_return_data(groupId);
        movie = m_moviesByID.search_and_return_data(movieId);
        if (movie->isVIP()) {
            if (!(group->is_VIP())) {
                return StatusType::FAILURE;
            }
        }
        if (group->get_numUsers() <= 0) {
            return StatusType::FAILURE;
        }
        Genre genre = movie->get_genre();
        group->group_watch(genre);
        int oldViews = movie->get_views();
        movie->add_view(group->get_numUsers());
        insert_and_remove(genre, movie, movieId, oldViews, movie->get_rating());
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    catch (const std::bad_alloc& e) {
        movie->add_view(-1);
        m_moviesByID.insert(movie, movieId);
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    switch (genre) {
        case (Genre::COMEDY):
            return m_comedyByRating.get_num_nodes();
        case Genre::DRAMA:
            return m_dramaByRating.get_num_nodes();
        case Genre::ACTION:
            return m_actionByRating.get_num_nodes();
        case Genre::FANTASY:
            return m_fantasyByRating.get_num_nodes();
        case Genre::NONE:
            return m_totalMovies;
    }
    return output_t<int>(StatusType::FAILURE);
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if (output == nullptr) {
        return StatusType::INVALID_INPUT;
    }
    try {
        switch (genre) {
            case Genre::COMEDY:
                m_comedyByRating.get_all_data(output);
                break;
            case Genre::DRAMA:
                m_dramaByRating.get_all_data(output);
                break;
            case Genre::ACTION:
                m_actionByRating.get_all_data(output);
                break;
            case Genre::FANTASY:
                m_fantasyByRating.get_all_data(output);
                break;
            case Genre::NONE:
                m_moviesByRating.get_all_data(output);
                break;
        }
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
	if(userId<=0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        User* user = m_usersByID.search_and_return_data(userId);
        return user->get_user_views(genre); 
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if(userId<=0|| movieId<=0 || rating<0 ||rating>100) {
        return StatusType::INVALID_INPUT;
    }
    try{
        User* user = m_usersByID.search_and_return_data(userId);
        Movie* movie = m_moviesByID.search_and_return_data(movieId);
        if(movie->isVIP()&&!user->isVIP()) {
            return StatusType::FAILURE;
        }
        double oldRating = movie->get_rating();
        movie->add_rating(rating);
        insert_and_remove(movie->get_genre(), movie, movieId, movie->get_views(), oldRating);
    }
    catch (const NodeNotFound& e) {
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
    if ( groupId <= 0) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    int id = 0;
    try {
        Group* group = m_groupsByID.search_and_return_data(groupId);
        if (group->get_numUsers() <= 0) {
            return output_t<int>(StatusType::FAILURE);
        }
        Genre genre = group->find_max();
        switch (genre) {
            case Genre::COMEDY:
                id = m_comedyByRating.get_max();
                break;
            case Genre::DRAMA:
                id = m_dramaByRating.get_max();
                break;
            case Genre::ACTION:
                id = m_actionByRating.get_max();
                break;
            case Genre::FANTASY:
                id = m_fantasyByRating.get_max();
                break;
            default:
                break;
        }
    }
    catch (const NodeNotFound& e) {
        return output_t<int>(StatusType::FAILURE);
    }
    if (id == -1) {
        return output_t<int>(StatusType::FAILURE);
    }
    return id;
}


//---------------------------------------Private Helper Functions---------------------------------------------

void streaming_database::insert_and_remove(const Genre genre, Movie* movie, const int movieId, const int oldViews,
                                                                                                 const double oldRating)
{
    switch (genre) {
        case Genre::COMEDY:
            m_comedyByRating.remove(movieId, oldViews, oldRating);
            m_comedyByRating.insert(movie, movieId, movie->get_views(), movie->get_rating());
            break;
        case Genre::DRAMA:
            m_dramaByRating.remove(movieId, oldViews, oldRating);
            m_dramaByRating.insert(movie, movieId, movie->get_views(), movie->get_rating());
            break;
        case Genre::ACTION:
            m_actionByRating.remove(movieId, oldViews, oldRating);
            m_actionByRating.insert(movie, movieId, movie->get_views(), movie->get_rating());
            break;
        case Genre::FANTASY:
            m_fantasyByRating.remove(movieId, oldViews, oldRating);
            m_fantasyByRating.insert(movie, movieId, movie->get_views(), movie->get_rating());
            break;
        default:
            break;
    }
    m_moviesByRating.remove(movieId, oldViews, oldRating);
    m_moviesByRating.insert(movie, movieId, movie->get_views(), movie->get_rating());
}

void streaming_database::remove_by_genre(Movie* movie, const int movieId)
{
    switch (movie->get_genre()) {
        case Genre::COMEDY:
            m_comedyByRating.remove(movieId, movie->get_views(), movie->get_rating());
            break;
        case Genre::DRAMA:
            m_dramaByRating.remove(movieId, movie->get_views(), movie->get_rating());
            break;
        case Genre::ACTION:
            m_actionByRating.remove(movieId, movie->get_views(), movie->get_rating());
            break;
        case Genre::FANTASY:
            m_fantasyByRating.remove(movieId, movie->get_views(), movie->get_rating());
            break;
        default:
            break;
    }
}