#ifndef USER_H
#define USER_H

class Group;

#include "wet1util.h"
#include "Group.h"

/*
* Class User
* This class is used to represent a user in the streaming system.
*/
class User {
public:

    /*
    * Constructor of User class
    * @param name - The user's ID and whether the user is a VIP user
    * @return - A new instance of User
    */
    User(const int userID, const bool isVIP);

    /*
    * Copy Constructor and Assignment Operator of User class
    * streaming does not allow two of the same user or group (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    * Additionally, we ensure that internal fields are not initialized as garbage values by deleting the default
    *   constructor.
    */
    User() = delete;
    User(const User& other) = delete;
    User& operator=(const User& other) = delete;

    /*
    * Destructor of User class
    * @param - none
    * @return - void
    */
    ~User();

    /*
    * Return the user's ID
    * @return - integer of the user's ID
    */
    int get_userId() const;

    /*
    * Return whether the user is a VIP
    * @return - bool - true if the user is a VIP, otherwise false
    */
    bool isVIP() const;

    /*
    * Return a pointer to the group that the user belongs to
    * @return - Group*, the group of the user
    */
    Group* get_group() const;

    /*
    * Helper function
    * Returns the array of views
    * @param
    * @return - int*, array of views
    */
    int get_user_views(const Genre genre) const;

    /*
    * Update the group that the user is a part of
    * @param - pointer to the group that the user is a part of
    * @return - void
    */
    void update_group(Group* tmpGroup);

    /*
    * Reset the user's group to nullptr
    * @param - none
    * @return - void
    */
    void remove_group();

    /*
    * Increase the user's views of a certain genre
    * @param - genre (index) of the array to increase
    * @return - void
    */
    void add_view(const Genre genre);

    /*
    * Calculates and updates the total views of the user, by adding his group views
    * @param - none
    * @return - void
    */
    void update_views();

private:

    /*
     * The internal fields of User:
     *   The user's ID
     *   The user's views according to genre
     *   Whether or not the user is a VIP user
     *   A pointer to the user's group
     */   
    int m_userID;
    int m_userViews[4];
    bool m_VIP;
    Group* m_group;

    friend class Group;
};

#endif //USER_H