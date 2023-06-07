#include "User.h"

//--------------------------------Constructor and Destructor------------------------------------

User::User(const int userID, const bool isVIP):
    m_userID(userID),
    m_userViews{0},
    m_VIP(isVIP),
    m_group(nullptr) 
{}

User::~User()
{
    m_group = nullptr;
}

//---------------------------------------Getters---------------------------------------------

int User::get_userId() const
{
    return m_userID;
}

bool User::isVIP() const
{
    return m_VIP;
}

Group* User::get_group() const
{
    return m_group;
}

int User::get_user_views(const Genre genre) const {
    int views = 0;
    if (genre == Genre::NONE) {
        for (int i = 0; i < 4; i++) {
            views += m_userViews[i];
        }
    }
    else {
        views = m_userViews[static_cast<int>(genre)];
    }
    if (m_group != nullptr) {
        views += m_group->get_group_views(genre);
    }
    return views;
}

//---------------------------------------Setters---------------------------------------------

void User::update_group(Group* tmpGroup) 
{
    m_group = tmpGroup;
}

void User::remove_group()
{
    m_group = nullptr;
}

void User::add_view(const Genre genre)
{
    m_userViews[static_cast<int>(genre)]++;
    if (m_group != nullptr) {
        m_group->add_view(genre);
    }
}

void User::update_views()
{
    for (int i = 0; i < 4; i++) {
        m_userViews[i] += m_group->get_group_views(static_cast<Genre>(i));
    }
}