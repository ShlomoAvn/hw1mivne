import random
import string
import os


def testGenerator():
    for i in range(1, 10000):
        listGroups = [-1]
        listUsers = [-1]
        listMovies = [-1]
        usersInGroup = {}
        checkInGroup = {}
        genre = [0, 1, 2, 3, 4]
        lenG = 0
        lenU = 0
        lenM = 0
        with open("inFilesMe/test" + str(i) + ".in", 'w') as file:
            with open("outFilesDesired/output" + str(i) + ".out", 'w') as outFile:
                for j in range(1, 1000):
                    num = random.randint(0, 3)
                    if num == 0:
                        file.writelines(["add_group ", str(j), "\n"])
                        if lenG == 0:
                            listGroups.pop(0)
                            lenG = 1
                        listGroups.append(j)
                        usersInGroup[j] = list()
                        outFile.writelines(["add_group: SUCCESS\n"])
                    elif num == 1:
                        userID = j
                        file.writelines(["add_user ", str(userID), " ", str(bool(random.randint(0, 1))).capitalize(), "\n"])
                        if lenU == 0:
                            listUsers.pop(0)
                            lenU = 1
                        listUsers.append(userID)
                        checkInGroup[userID] = False
                        outFile.writelines(["add_user: SUCCESS\n"])
                    elif num == 2:
                        movieID = j+1
                        random.shuffle(genre)
                        file.writelines(["add_movie ", str(movieID), " ", str(genre[0]), " ", str(j), " ", str(bool(random.randint(0, 1))).capitalize(), "\n"])
                        if lenM == 0:
                            listMovies.pop(0)
                            lenM = 1
                        listMovies.append(movieID)
                        outFile.writelines(["add_movie: SUCCESS\n"])
                for j in range(1, 400):
                    num = random.randint(0, 11)
                    if num == 0:
                        groupId = listGroups[random.randint(0, len(listGroups)-1)]
                        userId = listUsers[random.randint(0, len(listUsers)-1)]
                        file.writelines(["add_user_to_group ", str(userId), " ", str(groupId), "\n"])
                        if checkInGroup[userId] is False:
                            usersInGroup[groupId].append(userId)
                            checkInGroup[userId] = True
                            outFile.writelines(["add_user_to_group: SUCCESS\n"])
                        else:
                            outFile.writelines(["add_user_to_group: FAILURE\n"])
                    elif num == 1:
                        file.writelines(["user_watch ", str(listUsers[random.randint(0, len(listUsers)-1)]), " ", str(listMovies[random.randint(0, len(listMovies)-1)]), "\n"])
                        outFile.writelines(["user_watch: \n"])
                    elif num == 2:
                        file.writelines(["group_watch ", str(listGroups[random.randint(0, len(listGroups)-1)]), " ", str(listMovies[random.randint(0, len(listMovies)-1)]), "\n"])
                        outFile.writelines(["group_watch: \n"])
                    elif num == 3:
                        random.shuffle(genre)
                        file.writelines(["get_all_movies_count ", str(genre[0]), "\n"])
                        outFile.writelines(["get_all_movies_count: \n"])
                    elif num == 4:
                        random.shuffle(genre)
                        file.writelines(["get_all_movies ", str(genre[0]), "\n"])
                        outFile.writelines(["get_all_movies: \n"])
                    elif num == 5:
                        random.shuffle(genre)
                        file.writelines(["get_num_views ", str(listUsers[random.randint(0, len(listUsers)-1)]), " ", str(genre[0]), "\n"])
                        outFile.writelines(["get_num_views: \n"])
                    elif num == 6:
                        file.writelines(["rate_movie ", str(listUsers[random.randint(0, len(listUsers)-1)]), " ", str(listMovies[random.randint(0, len(listMovies)-1)]), " ", str(random.randint(0, 100)), "\n"])
                        outFile.writelines(["rate_movie: \n"])
                    elif num == 7:
                        file.writelines(["get_group_recommendation ", str(listGroups[random.randint(0, len(listGroups)-1)]), "\n"])
                        outFile.writelines(["get_group_recommendation: \n"])
                    elif num == 8:
                        movieId = listMovies[random.randint(0, len(listMovies)-1)]
                        file.writelines(["remove_movie ", str(movieId), "\n"])
                        listMovies.remove(movieId)
                        outFile.writelines(["remove_movie: SUCCESS\n"])
                    elif num == 9:
                        userID = listUsers[random.randint(0, len(listUsers)-1)]
                        file.writelines(["remove_user ", str(userID), "\n"])
                        listUsers.remove(userID)
                        outFile.writelines(["remove_user: SUCCESS\n"])
                    elif num == 10:
                        groupID = listGroups[random.randint(0, len(listGroups)-1)]
                        file.writelines(["remove_group ", str(groupID), "\n"])
                        listGroups.remove(groupID)
                        outFile.writelines(["remove_group: SUCCESS\n"])

    pass


def main():
    testGenerator()

if __name__ == "__main__":
    main()