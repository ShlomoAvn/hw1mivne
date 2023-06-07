#ifndef GROUP_H
#define GROUP_H

class User;

#include "User.h"
#include "wet1util.h"
#include "GenericNode.h"
#include "AVLTree.h"


/*
* Class Group
* This class is used to represent a group of users in the streaming system.
*/
class Group {
public:

    /*
    * Default constructor of Group class - creates an empty group with no users
    * @param - the group's ID
    * @return - A new instance of Group
    */
    Group(const int groupID);

    /*
    * Copy Constructor and Assignment Operator of Group class
    * streaming does not allow two of the same user or group (repeating ID's).
    * Therefore the system does not allow a copy constructor or assignment operator.
    * Additionally, we ensure that internal fields are not initialized as garbage values by deleting the default
    *   constructor.
    */
    Group() = delete;
    Group(const Group& other) = delete;
    Group& operator=(const Group& other) = delete;

    /*
    * Destructor of Group class
    * @param - none
    * @return - void
    */
    ~Group() = default;

    /*
    * Return whether the group is a VIP group
    * @return - bool - true if the user is a VIP, otherwise false
    */
    bool is_VIP() const;

    /*
    * Return the number of users in the group
    * @return - int
    */
    int get_numUsers() const;

    /*
    * Adds a view to a genre when a group user watches a movie
    * @return - int
    */
    void add_view(const Genre genre);

    /*
    * Adds a view to a genre when the group watches a movie
    * @return - int
    */
    void group_watch(const Genre genre);

    /*
    * Helper function for add_user_to_group in streaming:
    * Adds the user to the group and updates the group's stats accordingly
    * @param - a pointer to the user to be added, its ID, views, and whether it is a VIP
    * @return - void
    */
    void add_user(User* user, const int userID, bool VIP);

    /*
    * Helper function for remove_user in streaming:
    * Removes the user from the group and updates the group's stats accordingly
    * @param - a pointer to the user to be removed, its ID, and whether it is a VIP
    * @return - StatusType
    */
    StatusType remove_user(User* user, const int userID, bool VIP);

    /*
    * Helper function for remove_group in streaming:
    * Removes the users from the group
    * @param - none
    * @return - void
    */
    void remove_group();

    int get_group_views(const Genre genre) const;

    Genre find_max() const;

private:

    /*
     * The internal fields of group:
     *   The group's ID
     *   The number of VIP users the group has
     *   The number of users the group has
     *   The total number of movie views the group has according to genre
     *   The number of movie views the group has as a group, according to genre
     *   An AVL tree of the group's users organized by their ID numbers
     */
    int m_id;
    int m_VIP;
    int m_numUsers;
    int m_totalViews[4];
    int m_groupViews[4];
    Tree<GenericNode<User*>, User*> m_usersByID;

};

#endif //GROUP_H