//
// Created by Tnecesoc on 2017/3/8.
//

#ifndef SPORTMANAGER_SPLAY_ENTITY_H
#define SPORTMANAGER_SPLAY_ENTITY_H

#include <stdbool.h>
#include <mem.h>
#include <malloc.h>
#include "Utils/Algorithm.h"
#include "Utils/ArrayMatrix.h"

#define MAX_SPORT_FIELD_NAME_LEN 32
#define MAX_TEAM_NAME_LEN 32
#define MAX_ATHLETE_NAME_LEN 32
#define MAX_MATCH_NAME_LEN 32
#define MAX_TOURNAMENT_NAME_LEN 128

#define MAX_MATCH_PER_ATHLETE 3

#define MIN_MEN_PER_MATCH 4

#define MAN_STATUS_NOT_PARTICIPATED 0
#define MAN_STATUS_MATCH_NOT_FINISHED -1

#define MAN_SEX_FEMALE 0
#define MAN_SEX_MALE 1

#define MATCH_NO_LIMIT 0
#define MATCH_MALE_ONLY 1
#define MATCH_FEMALE_ONLY 2

#define MATCH_STATUS_CANCELED -1

#define MATCH_SCORE_TYPE_DISTANCE 1
#define MATCH_SCORE_TYPE_TIME 2

#define TOURNAMENT_STATUS_CONSTRUCTING -3
#define TOURNAMENT_STATUS_DESIGNING -2
#define TOURNAMENT_STATUS_PENDING -1
#define TOURNAMENT_STATUS_RUNNING 0
#define TOURNAMENT_STATUS_ENDED 1

#define TIME_AM_START(day) (510 + 1440 * ((day) - 1))
#define TIME_AM_END(day) (690 + 1440 * ((day) - 1))
#define TIME_PM_START(day) (840 + 1440 * ((day) - 1))
#define TIME_PM_END(day) (1020 + 1440 * ((day) - 1))

typedef struct {

    // Constructing
    char name[MAX_SPORT_FIELD_NAME_LEN];
    int capacity;
} SportField;

typedef struct {

    // Designing
    char name[MAX_TEAM_NAME_LEN];

    // Ended
    int score;
} Team;

typedef struct {

    // Pending
    char name[MAX_ATHLETE_NAME_LEN];
    int sex;
    int teamId;

    // Ended
    int score;
} Athlete;

typedef struct {

    // Designing
    char name[MAX_MATCH_NAME_LEN];
    double roundTime;
    int fieldId;
    int scoreType;
    int maxMen;

    int sexLimit;

    // Pending
    double startAt, endAt;

    // Running
    int menCnt;
    // for player playerIds[i], i % capacity of fieldId is its position.
    ArrayList *playerIds;
    ArrayList *roundStartAt;

    // Ended
    ArrayList *hiScore;
//    int hiScore[MAX_ATHLETE_PER_TOURNAMENT];
} Match;

typedef struct {

    int status;

    char name[MAX_TOURNAMENT_NAME_LEN];

    int menCnt, teamsCnt, matchesCnt, fieldCnt;

    ArrayList *fields, *teams, *matches, *men;

//    SportField fields[MAX_FIELD_PER_TOURNAMENT];
//    Team teams[MAX_TEAM_PER_TOURNAMENT];
//    Match matches[MAX_MATCH_PER_TOURNAMENT];
//    Athlete men[MAX_MATCH_PER_TOURNAMENT];

    /**
     * For every a[i][j],
     * a[i][j] = 0 means i didn't participate in j,
     * a[i][j] < 0 means i participated in j but have no score yet,
     * a[i][j] > 0 means i participated in j and have its score set.
     */
//    int menMatches[MAX_ATHLETE_PER_TOURNAMENT][MAX_MATCH_PER_TOURNAMENT];
    ArrayMatrix *menMatches;
} Tournament;

// Basic
void resetTournament(Tournament *which, char *name);
int commit(Tournament *which);

// Constructing stage
bool addField(Tournament *which, char *name, int capacity);
bool setField(Tournament *which, int id, char *name, int capacity);
bool delField(Tournament *which, int id);

int onCommitConstruction(Tournament *which);

// Designing Stage
bool addTeam(Tournament *which, char *name);
bool setTeam(Tournament *which, int id, char *name);
bool delTeam(Tournament *which, int id);
bool addMatch(Tournament *which, char *name, int fieldId, double matchTime, int scoreType, int maxMen, int sexLimit);
bool setMatch(Tournament *which, int id, char *name, int fieldId, double matchTime, int scoreType, int maxMen, int sexLimit);
bool delMatch(Tournament *which, int id);

int onCommitDesign(Tournament *which);

// Pending Stage
bool addAthlete(Tournament *which, char *name, int sex, int teamId);
bool setAthlete(Tournament *which, int id, char *name, int sex, int teamId);
bool delAthlete(Tournament *which, int id);
int matchMenCount(const Tournament *which, int matchId);
int manMatchesCount(const Tournament *which, int manId);
bool canManPlayMore(const Tournament *which, int manId);
bool isMatchAvailable(const Tournament *which, int matchId);
bool setParticipation(Tournament *which, int manId, int matchId, bool participate);

int onCommitPending(Tournament *which);

// Running Stage
bool setScore(Tournament *which, int manId, int matchId, int score);

bool isMatchFinished(const Tournament *which, int matchId);
int scoreOfKthMan(const Tournament *which, int matchId, int k);

int onCommitRunning(Tournament *which);

void resetTournament(Tournament *which, char *name) {

    strcpy(which->name, name);

    which->status = TOURNAMENT_STATUS_CONSTRUCTING;

    which->fieldCnt = 0;
    which->teamsCnt = 0;
    which->menCnt = 0;
    which->matchesCnt = 0;

    if (!arrayListEmpty(which->fields)) {
        arrayListClear(which->fields);
    }

    if (!arrayListEmpty(which->teams)) {
        arrayListClear(which->teams);
    }

    if (!arrayListEmpty(which->men)) {
        arrayListClear(which->men);
    }

    if (!arrayListEmpty(which->matches)) {

        Match *match;

        for (int i = 0; i < which->matchesCnt; ++i) {
            match = arrayListGet(which->matches, i);

            if (match->roundStartAt != NULL) {
                arrayListClear(match->roundStartAt);
            }

            if (match->playerIds != NULL) {
                arrayListClear(match->playerIds);
            }

            if (match->hiScore != NULL) {
                arrayListClear(match->hiScore);
            }

            free(match->playerIds);
            free(match->hiScore);
        }

        arrayListClear(which->matches);
    }

    if (!matrixEmpty(which->menMatches)) {
        matrixClear(which->menMatches);
    }
}

#define COMMIT_SUCCEED 0x01
#define COMMIT_FAILED 0x00

#define COMMIT_NO_FIELD 0x02
#define COMMIT_NO_TEAM 0x04

#define COMMIT_NO_MATCH 0x08

#define COMMIT_NO_ATHLETE 0x10

#define COMMIT_HAVE_CANCELED_MATCHES 0x20

#define COMMIT_MATCH_NOT_ALL_FINISHED 0x40

#define COMMIT_MATCH_ENDED 0x80

int commit(Tournament *which) {

    if (which->status <= TOURNAMENT_STATUS_CONSTRUCTING) {
        return onCommitConstruction(which);
    } else if (which->status == TOURNAMENT_STATUS_DESIGNING) {
        return onCommitDesign(which);
    } else if (which->status == TOURNAMENT_STATUS_PENDING) {
        return onCommitPending(which);
    } else if (which->status == TOURNAMENT_STATUS_RUNNING) {
        return onCommitRunning(which);
    } else if (which->status >= TOURNAMENT_STATUS_ENDED) {
        return COMMIT_MATCH_ENDED | COMMIT_FAILED;
    }

    return COMMIT_FAILED;
}

bool addField(Tournament *which, char *name, int capacity) {

    if (which->status != TOURNAMENT_STATUS_CONSTRUCTING) {
        return false;
    }

    SportField tmp;

    strcpy(tmp.name, name);
    tmp.capacity = capacity;

    arrayListAdd(which->fields, &tmp);

    which->fieldCnt = arrayListSize(which->fields);

    return true;
}

bool setField(Tournament *which, int fieldId, char *name, int capacity) {

    if (which->status != TOURNAMENT_STATUS_CONSTRUCTING || fieldId < 0 || fieldId >= which->fieldCnt) {
        return false;
    }

    SportField *field = (SportField *) arrayListGet(which->fields, fieldId);

    strcpy(field->name, name);
    field->capacity = capacity;

    return true;
}

bool delField(Tournament *which, int fieldId) {

    if (which->status != TOURNAMENT_STATUS_CONSTRUCTING || fieldId < 0 || fieldId >= which->fieldCnt) {
        return false;
    }

    arrayListDelete(which->fields, fieldId);

    which->fieldCnt = arrayListSize(which->fields);

    return true;
}

int onCommitConstruction(Tournament *which) {
    if (which->fieldCnt > 0) {
        which->status = TOURNAMENT_STATUS_DESIGNING;
        return COMMIT_SUCCEED;
    } else {
        return COMMIT_NO_FIELD;
    }
}

bool addTeam(Tournament *which, char *name) {

    if (which->status != TOURNAMENT_STATUS_DESIGNING) {
        return false;
    }

    Team tmp;

    strcpy(tmp.name, name);
    tmp.score = 0;

    arrayListAdd(which->teams, &tmp);

    which->teamsCnt = arrayListSize(which->teams);

    return true;
}

bool setTeam(Tournament *which, int id, char *name) {

    if (which->status != TOURNAMENT_STATUS_DESIGNING || id < 0 || id >= which->teamsCnt) {
        return false;
    }

    Team *team = arrayListGet(which->teams, id);

    strcpy(team->name, name);
    team->score = 0;
    return true;
}

bool delTeam(Tournament *which, int id) {

    if (which->status != TOURNAMENT_STATUS_DESIGNING || id < 0 || id >= which->teamsCnt) {
        return false;
    }

    arrayListDelete(which->teams, id);
    which->teamsCnt = arrayListSize(which->teams);
    return true;
}

bool addMatch(Tournament *which, char *name, int fieldId, double matchTime, int scoreType, int maxMen, int sexLimit) {

    if (which->status != TOURNAMENT_STATUS_DESIGNING) {
        return false;
    }

    Match tmp;
    strcpy(tmp.name, name);
    tmp.fieldId = fieldId;
    tmp.roundTime = matchTime > 180 ? 180 : (matchTime < 0 ? 0 : matchTime);
    tmp.scoreType = scoreType;
    tmp.maxMen = maxMen;
    tmp.sexLimit = sexLimit;

    tmp.playerIds = NULL;
    tmp.roundStartAt = NULL;
    tmp.hiScore = NULL;

    arrayListAdd(which->matches, &tmp);

    matrixAddRow(which->menMatches, NULL);

    which->matchesCnt = arrayListSize(which->matches);

    return true;
}

bool setMatch(Tournament *which, int id, char *name, int fieldId, double matchTime, int scoreType, int maxMen, int sexLimit) {

    if (which->status != TOURNAMENT_STATUS_DESIGNING || id < 0 || id >= which->matchesCnt) {
        return false;
    }

    Match *match = arrayListGet(which->matches, id);

    strcpy(match->name, name);
    match->fieldId = fieldId;
    match->roundTime = matchTime > 180 ? 180 : (matchTime < 0 ? 0 : matchTime);
    match->scoreType = scoreType;
    match->maxMen = maxMen;
    match->sexLimit = sexLimit;

    match->roundStartAt = match->playerIds = match->hiScore = NULL;

    return true;
}

bool delMatch(Tournament *which, int id) {

    if (which->status != TOURNAMENT_STATUS_DESIGNING || id < 0 || id >= which->matchesCnt) {
        return false;
    }

    arrayListDelete(which->matches, id);
    matrixDeleteRow(which->menMatches, id);

    which->matchesCnt = arrayListSize(which->matches);
    return true;
}

int roundNeededForMatch(const Tournament *which, int matchId) {

    Match *match = arrayListGet(which->matches, matchId);

    SportField *field = arrayListGet(which->fields, match->fieldId);

    return match->maxMen / field->capacity + match->maxMen % field->capacity;
}

double timeNeededForMatch(const Tournament *which, int matchId) {

    Match *match = arrayListGet(which->matches, matchId);

    return roundNeededForMatch(which, matchId) * match->roundTime;
}

Tournament *____which;

bool ___cmp_finish_earlier(const _M a, const _M b) {

    Match *ma, *mb;

    ma = arrayListGet(____which->matches, a);
    mb = arrayListGet(____which->matches, b);

    if (ma->fieldId == mb->fieldId) {
        return timeNeededForMatch(____which, a) < timeNeededForMatch(____which, b);
    } else {
        return ma->fieldId < mb->fieldId;
    }
}

// 需要初始化 which.matches[matchId].startAt
double scheduleMatchAt(Tournament *which, int matchId) {

    Match *match = arrayListGet(which->matches, matchId);

    double startAt = match->startAt;
    match->roundStartAt = newArrayList(sizeof(double));

    int round = roundNeededForMatch(which, matchId);
    int days = 1;
    for (int i = 1; i <= round; ++i) {

        arrayListAdd(match->roundStartAt, &startAt);

        if (startAt <= TIME_AM_END(days)) { // 第 i 回合在上午开始
            if (startAt + match->roundTime <= TIME_AM_END(days)) {
                startAt += match->roundTime; // 上午比得完就上午比
            } else {
                startAt = TIME_PM_START(days) + match->roundTime; // 上午比不完推迟到下午再比
            }
        } else { // 第 i 回合在下午开始
            if (startAt + match->roundTime <= TIME_PM_END(days)) {
                startAt += match->roundTime; // 下午比完就下午比
            } else {
                startAt = TIME_AM_END(++days) + match->roundTime; // 下午比不完就推到第二天上午比
            }
        }
    }

    // 返回所有回合结束时的时间
    return startAt;
}

void schedule(Tournament *which) {

    int *ids = malloc(which->matchesCnt * sizeof(int));

    for (int i = 0; i < which->matchesCnt; ++i) {
        ids[i] = i;
    }
    ____which = which;
    sortWithComparator(ids, ids + which->matchesCnt, ___cmp_finish_earlier);

    Match *prev_match;
    Match *match = arrayListGet(which->matches, 0);

    match->startAt = TIME_AM_START(1);
    match->endAt = scheduleMatchAt(which, ids[0]);

    for (int i = 1; i < which->matchesCnt; ++i) {
        prev_match = match;
        match = arrayListGet(which->matches, i);

        if (match->fieldId == prev_match->fieldId) {
            match->startAt = prev_match->endAt;
        } else {
            match->startAt = TIME_AM_START(1);
        }

        match->endAt = scheduleMatchAt(which, ids[i]);
    }

    free(ids);
    ____which = NULL;
}

int onCommitDesign(Tournament *which) {

    int res = 0;

    if (which->teamsCnt <= 0) {
        res |= COMMIT_NO_TEAM;
    }

    if (which->matchesCnt <= 0) {
        res |= COMMIT_NO_MATCH;
    }

    if (which->matchesCnt > 0 && which->teamsCnt > 0) {
        res |= COMMIT_SUCCEED;
        which->status = TOURNAMENT_STATUS_PENDING;
        schedule(which);
    }

    return res;
}

bool addAthlete(Tournament *which, char *name, int sex, int teamId) {

    if (which->status != TOURNAMENT_STATUS_PENDING) {
        return false;
    }

    Athlete tmp;

    strcpy(tmp.name, name);
    tmp.sex = sex;
    tmp.teamId = teamId;
    tmp.score = 0;

    int markJoinNoMatch = MAN_STATUS_NOT_PARTICIPATED;

    arrayListAdd(which->men, &tmp);
    matrixAddCell(which->menMatches, &markJoinNoMatch);

    which->menCnt = arrayListSize(which->men);

    return true;
}

bool setAthlete(Tournament *which, int id, char *name, int sex, int teamId) {

    if (which->status != TOURNAMENT_STATUS_PENDING || id < 0 || id >= which->menCnt) {
        return false;
    }

    Athlete *tmp = arrayListGet(which->men, id);

    strcpy(tmp->name, name);
    tmp->sex = sex;
    tmp->teamId = teamId;
    tmp->score = 0;

    return true;
}

bool delAthlete(Tournament *which, int id) {

    if (which->status != TOURNAMENT_STATUS_PENDING || id < 0 || id >= which->menCnt) {
        return false;
    }

    arrayListDelete(which->men, id);
    matrixDeleteCell(which->menMatches, id);
    which->menCnt = arrayListSize(which->men);

    return true;
}

int matchMenCount(const Tournament *which, int matchId) {

    int ret = 0;

    Match *match = arrayListGet(which->matches, matchId);

    for (int i = 0; i < which->menCnt; ++i) {
        if (*(int *) matrixGet(which->menMatches, matchId, i) != MAN_STATUS_NOT_PARTICIPATED) {
            ret++;
            if (ret >= match->maxMen) {
                return ret;
            }
        }
    }

    return ret;
}

int manMatchesCount(const Tournament *which, int manId) {

    int ret = 0;

    for (int i = 0; i < which->matchesCnt; ++i) {
        if (*(int *) matrixGet(which->menMatches, i, manId) != MAN_STATUS_NOT_PARTICIPATED) {
            ret++;
            if (ret >= MAX_MATCH_PER_ATHLETE) {
                return ret;
            }
        }
    }

    return ret;
}

bool isMatchAvailable(const Tournament *which, int matchId) {

    Match *match = arrayListGet(which->matches, matchId);

    return matchMenCount(which, matchId) < match->maxMen;
}

bool isSexLimitSatisfied(const Tournament *which, int manId, int matchId) {

    Athlete *man = arrayListGet(which->men, manId);

    Match *match = arrayListGet(which->matches, matchId);

    return match->sexLimit == MATCH_NO_LIMIT
           || (man->sex == MAN_SEX_FEMALE && match->sexLimit == MATCH_FEMALE_ONLY)
           || (man->sex == MAN_SEX_MALE && match->sexLimit == MATCH_MALE_ONLY);
}

bool canManPlayMore(const Tournament *which, int manId) {
    return manMatchesCount(which, manId) < MAX_MATCH_PER_ATHLETE;
}

bool isTimeConflicted(const Tournament *which, int matchAId, int matchBId) {

    Match *ma = arrayListGet(which->matches, matchAId);
    Match *mb = arrayListGet(which->matches, matchBId);

    return ma->startAt <= mb->endAt && mb->startAt <= ma->endAt;
}

bool isTimePermiting(const Tournament *which, int manId, int matchId) {

    for (int i = 0; i < which->matchesCnt; ++i) {
        if (*(int *) matrixGet(which->menMatches, manId, i) != MAN_STATUS_NOT_PARTICIPATED) {
            if (isTimeConflicted(which, i, matchId)) {
                return false;
            }
        }
    }

    return true;
}

bool setParticipation(Tournament *which, int manId, int matchId, bool participate) {

    if (participate) {
        if (canManPlayMore(which, manId)
            && isMatchAvailable(which, matchId)
            && isSexLimitSatisfied(which, manId, matchId)
            && isTimePermiting(which, manId, matchId)) {

            *(int *) matrixGet(which->menMatches, matchId, manId) = MAN_STATUS_MATCH_NOT_FINISHED;
        } else {
            return false;
        }
    } else {
        *(int *) matrixGet(which->menMatches, manId, matchId) = MAN_STATUS_NOT_PARTICIPATED;
    }

    return true;
}

int onCommitPending(Tournament *which) {

    if (which->menCnt <= 0) {
        return COMMIT_FAILED | COMMIT_NO_ATHLETE;
    }

    Match *match;

    int res = 0;
    for (int i = 0; i < which->matchesCnt; ++i) {
        match = arrayListGet(which->matches, i);
        match->playerIds = newArrayList(sizeof(int));
        match->hiScore = newArrayList(sizeof(int));
        for (int j = 0; j < which->menCnt; j++) {
            if (*(int *) matrixGet(which->menMatches, i, j) != MAN_STATUS_NOT_PARTICIPATED) {
                arrayListAdd(match->playerIds, &j);
                arrayListAdd(match->hiScore, matrixGet(which->menMatches, i, j));
            }
        }

        match->menCnt = arrayListSize(match->playerIds);

        if (match->menCnt < MIN_MEN_PER_MATCH) {
            arrayListClear(match->playerIds);
            match->playerIds = NULL;
            match->menCnt = MATCH_STATUS_CANCELED;
            res |= COMMIT_HAVE_CANCELED_MATCHES;
        }
    }

    res |= COMMIT_SUCCEED;
    which->status = TOURNAMENT_STATUS_RUNNING;

    return res;
}

bool setScore(Tournament *which, int manId, int matchId, int score) {
    if (which->status == TOURNAMENT_STATUS_RUNNING) {
        if (*(int *) matrixGet(which->menMatches, matchId, manId) == MAN_STATUS_MATCH_NOT_FINISHED) {
            *(int *) matrixGet(which->menMatches, matchId, manId) = score;
            return true;
        } else {
            return false;
        }
    }
    return false;
}

int scoreOfKthMan(const Tournament *which, int matchId, int k) {

    if (which->status < TOURNAMENT_STATUS_RUNNING) {
        return 0;
    }

    Match *match = arrayListGet(which->matches, matchId);

    if (arrayListSize(match->playerIds) <= 6) {
        switch (k) {
            case 0:
                return 5;
            case 1:
                return 3;
            case 2:
                return 2;
            default:
                return 0;
        }
    } else {
        switch (k) {
            case 0:
                return 7;
            case 1:
                return 5;
            case 2:
                return 3;
            case 3:
                return 2;
            case 4:
                return 1;
            default:
                return 0;
        }
    }

}

bool isMatchFinished(const Tournament *which, int matchId) {

    Match *match = arrayListGet(which->matches, matchId);

    if (match->menCnt == MATCH_STATUS_CANCELED) {
        return true;
    }

    for (int i = 0; i < which->menCnt; ++i) {
        if (*(int *) matrixGet(which->menMatches, matchId, i) == MAN_STATUS_MATCH_NOT_FINISHED) {
            return false;
        }
    }

    return true;
}

int ____matchId;

bool ____cmp_better_greater(const _M a, const _M b) {
    return *(int *) matrixGet(____which->menMatches, ____matchId, a) >
           *(int *) matrixGet(____which->menMatches, ____matchId, b);
}

bool ____cmp_better_less(const _M a, const _M b) {
    return *(int *) matrixGet(____which->menMatches, ____matchId, a) <
           *(int *) matrixGet(____which->menMatches, ____matchId, b);
}

int onCommitRunning(Tournament *which) {

    int res = 0;

    ____which = which;

    for (int i = 0; i < which->matchesCnt; ++i) {
        Match *match = arrayListGet(which->matches, i);
        if (!isMatchFinished(which, i)) {
            res |= COMMIT_MATCH_NOT_ALL_FINISHED;
            return res;
        } else if (match->menCnt == MATCH_STATUS_CANCELED
                   || *(int *)arrayListGet(match->hiScore, 0) >= 0) {
            continue;
        }

        ____matchId = i;
        sortWithComparator(match->playerIds->begin,
                           match->playerIds->end,
                           match->scoreType == MATCH_SCORE_TYPE_DISTANCE ?
                           ____cmp_better_greater : ____cmp_better_less
        );


        for (int j = 0; j < match->menCnt; ++j) {
            int manId = *(int *) arrayListGet(match->playerIds, j);
            Athlete *man = arrayListGet(which->men, manId);
            Team *team = arrayListGet(which->teams, man->teamId);
            *(int *)arrayListGet(match->hiScore, j) = *(int *)matrixGet(which->menMatches, i, manId);
            man->score += scoreOfKthMan(which, i, j);
            team->score += scoreOfKthMan(which, i, j);
        }
    }

    ____which = NULL;

    res |= COMMIT_SUCCEED;
    which->status = TOURNAMENT_STATUS_ENDED;

    return res;
}

#endif //SPORTMANAGER_SPLAY_ENTITY2_H
