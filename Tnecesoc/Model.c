//
// Created by Tnecesoc on 2017/3/5.
//

#include <stdbool.h>
#include <malloc.h>
#include <dirent.h>

#include "Entity.h"
#include "IO.h"

#include "Utils/Trie.h"

#ifndef SPORTMANAGER_SPLAY_MODEL_H
#define SPORTMANAGER_SPLAY_MODEL_H

#define USER_PATH "./USERDATA"

#define RECORD_FILE_SUFFIX "record"

Tournament *cache;

Trie *athletesIndex, *matchesIndex, *teamsIndex, *fieldsIndex;

ArrayList *previousTournaments, *topAthletesIdCache, *topTeamsIdCache;


void maintainCache(FILE *in) {

    char tmp[128];

    tmp[0] = '\0';

    resetTournament(cache, "");

    loadTournament(in, cache);

    strcpy(tmp, cache->name);

    if (tmp[0] != '\0') {
        arrayListAdd(previousTournaments, &tmp);
    }
}

void newTournamentOnCache(char *name) {

    resetTournament(cache, name);

}

void initModel() {

    _mkdir(USER_PATH);

    fieldsIndex = newTrie();
    athletesIndex = newTrie();
    matchesIndex = newTrie();
    teamsIndex = newTrie();

    cache = malloc(sizeof(Tournament));

    cache->fields = newArrayList(sizeof(SportField));
    cache->teams = newArrayList(sizeof(Team));
    cache->men = newArrayList(sizeof(Athlete));
    cache->matches = newArrayList(sizeof(Match));

    cache->menMatches = newMatrix(sizeof(int));

    previousTournaments = newArrayList(sizeof(T_STRING));
    topTeamsIdCache = newArrayList(sizeof(int));
    topAthletesIdCache = newArrayList(sizeof(int));

    readEachFileWithSuffixInPath(USER_PATH, RECORD_FILE_SUFFIX, maintainCache);

    newTournamentOnCache("new_record");
}

void onLoad() {

    char *name;

    if (cache->status >= TOURNAMENT_STATUS_DESIGNING) {

        TrieClear(fieldsIndex);

        for (int i = 0; i < cache->fieldCnt; ++i) {
            name = arrayListGet(cache->fields, i);
            TriePut(fieldsIndex, name, i);
        }
    }

    if (cache->status >= TOURNAMENT_STATUS_PENDING) {

        TrieClear(matchesIndex);
        TrieClear(teamsIndex);

        for (int i = 0; i < cache->matchesCnt; ++i) {
            name = arrayListGet(cache->matches, i);
            TriePut(matchesIndex, name, i);
        }

        for (int i = 0; i < cache->teamsCnt; ++i) {
            name = arrayListGet(cache->teams, i);
            TriePut(teamsIndex, name, i);
        }

    }

    if (cache->status >= TOURNAMENT_STATUS_RUNNING) {

        TrieClear(athletesIndex);

        for (int i = 0; i < cache->menCnt; ++i) {
            name = arrayListGet(cache->men, i);
            TriePut(athletesIndex, name, i);
        }
    }

    if (cache->status >= TOURNAMENT_STATUS_ENDED) {

        arrayListClear(topAthletesIdCache);
        arrayListClear(topTeamsIdCache);

        topAthleteIds(cache, topAthletesIdCache);
        topTeams(cache, topTeamsIdCache);
    }
}

bool loadTournamentsFromFile(char *filename) {
    FILE *in = NULL;

    char path[128];
    sprintf(path, "%s/%s.%s", USER_PATH, filename, RECORD_FILE_SUFFIX);

    if ((in = fopen(path, "r")) == NULL) {
        return false;
    }

    resetTournament(cache, filename);

    loadTournament(in, cache);

    onLoad();

    fclose(in);

    return strlen(cache->name) > 0;
}

void deleteTournamentOnFile(char *name) {
    char path[100];
    sprintf(path, "%s/%s.%s", USER_PATH, name, RECORD_FILE_SUFFIX);
    deleteFile(path);
}

bool saveTournamentToFile() {

    FILE *out = NULL;

    char path[100];
    sprintf(path, "%s/%s.%s", USER_PATH, cache->name, RECORD_FILE_SUFFIX);

    if ((out = fopen(path, "w")) == NULL) {
        return false;
    }

    saveTournament(out, cache);

    fclose(out);

    return true;
}

void closeModel() {

    arrayListClear(topTeamsIdCache);
    arrayListClear(topAthletesIdCache);
    arrayListClear(previousTournaments);

    TrieClear(fieldsIndex);
    TrieClear(athletesIndex);
    TrieClear(matchesIndex);
    TrieClear(teamsIndex);

    free(previousTournaments);
    free(topTeamsIdCache);
    free(topAthletesIdCache);

    free(fieldsIndex);
    free(athletesIndex);
    free(matchesIndex);
    free(teamsIndex);

    resetTournament(cache, "");

    free(cache->menMatches);

    free(cache->fields);
    free(cache->teams);
    free(cache->matches);
    free(cache->men);

    free(cache);
}

int findIdByFieldName(char *fieldName) {
    return TrieQuery(fieldsIndex, fieldName);
}

int findIdByTeamName(char *teamName) {
    return TrieQuery(teamsIndex, teamName);
}

int findIdByMatchName(char *matchName) {
    return TrieQuery(matchesIndex, matchName);
}

int findIdByAthleteName(char *athleteName) {
    return TrieQuery(athletesIndex, athleteName);
}

int commitOnCache() {

    int res = commit(cache);

    if (res & COMMIT_SUCCEED) {
        onLoad();
    }

    return res;
}

#endif //SPORTMANAGER_SPLAY_MODEL_H
