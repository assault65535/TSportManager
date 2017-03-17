//
// Created by Tnecesoc on 2017/3/5.
//

#include "Entity.h"
#include "Model.c"
#include "View.c"
#include "Authentication.h"

/**
 * 5YeLwf, name: Long_distance_sailing
 * 4G666X, name: Operation_sea_protection
 * PGYymO, name: Shiratsuyu
 */

bool isSaved;

bool isRootAuthenticated;

#define ___NEED_ROOT_AUTHENTICATION___ if(!isRootAuthenticated) { announceNotAuthorized(); return;}

#define ___IN_THE_NAME_OF___(who) if (!isRootAuthenticated && !isAdminAccessAccepted((who), curAdminKey)) { announceNotAuthorized(); return; }

char curAdminKey[256];

char buf1[128], buf2[256], buf3[2048];

void init() {
    initModel();
    initView();
    isSaved = true;
    isRootAuthenticated = false;
}

#define END_OF_INPUT "__END_OF_INPUT"

void multipleAdd(int what) {

    bool flag;
    int k = 0;

    switch (what) {
        case MENU_ITEM_FIND_FIELD:
            if ((flag = cache->status == TOURNAMENT_STATUS_CONSTRUCTING)) {
                toAddSportFieldsPage();
                int capacity;
                while (scanf("%s", buf1), strcmp(buf1, END_OF_INPUT)) {
                    scanf("%d", &capacity);
                    flag &= addField(cache, buf1, capacity);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_TEAM:
            if ((flag = cache->status == TOURNAMENT_STATUS_DESIGNING)) {
                toAddTeamsPage();
                while (scanf("%s", buf1), strcmp(buf1, END_OF_INPUT)) {
                    flag &= addTeam(cache, buf1);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_MATCH:
            if ((flag = cache->status == TOURNAMENT_STATUS_DESIGNING)) {
                toAddMatchesPage();
                int fieldId, scoreType, maxMen, sexLimit;
                double matchTime;
                while (scanf("%s", buf1), strcmp(buf1, END_OF_INPUT)) {
                    scanf("%d %lf %d %d %d", &fieldId, &matchTime, &scoreType, &maxMen, &sexLimit);
                    flag &= addMatch(cache, buf1, fieldId, matchTime, scoreType, maxMen, sexLimit);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_ATHLETE:
            if ((flag = cache->status == TOURNAMENT_STATUS_PENDING)) {
                toAddAthletesPage();
                int sex, teamId;
                while (scanf("%s", buf1), strcmp(buf1, END_OF_INPUT)) {
                    scanf("%d", &sex);
                    scanf("%d", &teamId);
                    flag &= addAthlete(cache, buf1, sex, teamId);
                    k += flag;
                }
            }
            break;
        default:
            return;
    }

    if (flag) {
        announceEditedKData(k);
    } else {
        announceOperationIllegal();
    }
}

void multipleEdit(int what) {

    bool flag;
    int id;
    int k = 0;

    switch (what) {
        case MENU_ITEM_FIND_FIELD:
            if ((flag = cache->status == TOURNAMENT_STATUS_CONSTRUCTING)) {
                toEditSportFieldsPage();
                int capacity;
                while (scanf("%d", &id), id >= 0) {
                    scanf("%s %d", buf1, &capacity);
                    flag &= setField(cache, id, buf1, capacity);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_TEAM:
            if ((flag = cache->status == TOURNAMENT_STATUS_DESIGNING)) {
                toEditTeamsPage();
                while (scanf("%d", &id), id >= 0) {
                    scanf("%s", buf1);
                    flag &= setTeam(cache, id, buf1);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_MATCH:
            if ((flag = cache->status == TOURNAMENT_STATUS_DESIGNING)) {
                toEditMatchesPage();
                int fieldId, scoreType, maxMen, sexLimit;
                double matchTime;
                while (scanf("%d", &id), id >= 0) {
                    scanf("%s %d %lf %d %d %d", buf1, &fieldId, &matchTime, &scoreType, &maxMen, &sexLimit);
                    flag &= setMatch(cache, id, buf1, fieldId, matchTime, scoreType, maxMen, sexLimit);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_ATHLETE:
            if ((flag = cache->status == TOURNAMENT_STATUS_PENDING)) {
                toEditAthletesPage();
                int sex, teamId;
                while (scanf("%d", &id), id >= 0) {
                    scanf("%s %d %d", buf1, &sex, &teamId);
                    flag &= setAthlete(cache, id, buf1, sex, teamId);
                    k += flag;
                }
            }
            break;
        default:
            return;
    }

    if (flag) {
        announceEditedKData(k);
    } else {
        announceOperationIllegal();
    }
}

void multipleDelete(int what) {

    bool flag = true;
    int id;
    int k = 0;

    switch (what) {
        case MENU_ITEM_FIND_FIELD:
            if ((flag = cache->status == TOURNAMENT_STATUS_CONSTRUCTING)) {
                toDeleteSportFieldsPage();
                while (scanf("%d", &id), id >= 0) {
                    flag &= delField(cache, id);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_TEAM:
            if ((flag = cache->status == TOURNAMENT_STATUS_DESIGNING)) {
                toDeleteTeamsPage();
                while (scanf("%d", &id), id >= 0) {
                    flag &= delTeam(cache, id);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_MATCH:
            if ((flag = cache->status == TOURNAMENT_STATUS_DESIGNING)) {
                toDeleteMatchesPage();
                while (scanf("%d", &id), id >= 0) {
                    flag &= delMatch(cache, id);
                    k += flag;
                }
            }
            break;
        case MENU_ITEM_FIND_ATHLETE:
            if ((flag = cache->status == TOURNAMENT_STATUS_PENDING)) {
                toDeleteAthletesPage();
                while (scanf("%d", &id), id >= 0) {
                    flag &= delAthlete(cache, id);
                    k += flag;
                }
            }
            break;
        default:
            break;
    }

    if (flag) {
        announceEditedKData(k);
    } else {
        announceOperationIllegal();
    }
}

char *___correctJoinRequest(FILE *in) {
    int manId, matchId, participation;

    fscanf(in, "%d %d %d\n", &manId, &matchId, &participation);

    sprintf(buf2, "运动员 #%d %s 运动项目 #%d", manId, participation ? "报名参加" : "取消参加", matchId);

    if (manId >= 0 && manId < cache->menCnt && matchId >= 0 && matchId < cache->matchesCnt && participation) {
        setParticipation(cache, manId, matchId, !participation);
    }

    if (manId >= cache->menCnt) {
        strcat(buf2, " --- 该运动员不存在");
    }

    if (matchId >= cache->matchesCnt) {
        strcat(buf2, " --- 该比赛项目不存在");
    }

    if (manId < cache->menCnt && !canManPlayMore(cache, manId)) {
        strcat(buf2, " --- 运动员不能参加更多比赛");
    }

    if (matchId < cache->matchesCnt && !isMatchAvailable(cache, matchId)) {
        strcat(buf2, " --- 比赛项目参赛人员已满");
    }

    if (manId < cache->matchesCnt && !isSexLimitSatisfied(cache, manId, matchId)) {
        strcat(buf2, " --- 违反比赛项目的性别限制");
    }

    if (manId < cache->menCnt && matchId < cache->matchesCnt && !isTimePermiting(cache, manId, matchId)) {
        strcat(buf2, " --- 与运动员的其他赛程冲突");
    }

    strcat(buf2, "\n");

    if (manId >= 0 && manId < cache->menCnt && matchId >= 0 && matchId < cache->matchesCnt) {
        setParticipation(cache, manId, matchId, participation != 0);
    }

    return buf2;
}

void multipleJoin() {
    toJoinMatchPage();
    int manId, matchId, participation;
    int want = 0, fact = 0;

    FILE *temp2;

    temp2 = fopen(TEMP_FILE2, "w");

    while (scanf("%d", &manId), manId >= 0) {
        scanf("%d %d", &matchId, &participation);
        want++;
        fact += setParticipation(cache, manId, matchId, participation != 0);
        fprintf(temp2, "%d %d %d\n", manId, matchId, participation);
    }

    fclose(temp2);

    announceEditedADataButBDataFailed(fact, want - fact);

    if (want != fact) {
        correctFile(TEMP_FILE2, TEMP_FILE, ___correctJoinRequest);
        openTempFileWithNotePad();
    }
}

void handleIndividualJoin(int manId) {

    Athlete *man = arrayListGet(cache->men, manId);

    ___IN_THE_NAME_OF___(man->name)

    toIndividualJoinPage();
    printAthletesAt(stdout, cache, manId);

    int want = 0, fact = 0;

    FILE *temp2;

    temp2 = fopen(TEMP_FILE2, "w");

    int matchId, participation;

    while (scanf("%d", &matchId), matchId >= 0) {
        scanf("%d", &participation);
        want++;
        fact += setParticipation(cache, manId, matchId, participation != 0);
        fprintf(temp2, "%d %d %d\n", manId, matchId, participation);
    }

    fclose(temp2);

    announceEditedADataButBDataFailed(fact, want - fact);

    if (want != fact) {
        correctFile(TEMP_FILE2, TEMP_FILE, ___correctJoinRequest);
        openTempFileWithNotePad();
    }

}

void handleJoin(int which) {

    if (cache->status != TOURNAMENT_STATUS_PENDING) {
        announceOperationIllegal();
        return;
    }

    switch (which) {
        case MENU_ITEM_JOIN_INDIVIDUAL:
            handleIndividualJoin(requestInteger(0, cache->menCnt));
            break;
        case MENU_ITEM_JOIN_MULTIPLE: ___NEED_ROOT_AUTHENTICATION___
            multipleJoin();
            break;
        default:
            return;
    }
}

void multipleSetScore(int matchId){

    if (cache->status != TOURNAMENT_STATUS_RUNNING) {
        announceOperationIllegal();
        return;
    }

    Match *match = arrayListGet(cache->matches, matchId);

    ___IN_THE_NAME_OF___(match->name)

    toSetScorePage();
    printMatchAt(stdout, cache, matchId);
    int raceId, manId, score;
    int k = 0;
    while (scanf("%d", &raceId), raceId >= 0) {
        scanf("%d", &score);
        manId = *(int *) arrayListGet(match->playerIds, raceId);
        k += setScore(cache, manId, matchId, score);
    }

    announceEditedKData(k);
}

void multipleAuthorize() {

    char name[128][128];
    toAuthorizePage();

    char buf[128];

    int count = 0;

    while (scanf("%s", buf), strcmp(buf, END_OF_INPUT) != 0) {
        strcpy(name[count++], buf);
    }

    generateAdminKey(name, count, TEMP_FILE);

    announceAdminKeyGenerated();
    openTempFileWithNotePad();
}

void ___function_print_file_name(void *o) {
    strcat(buf3, (char *) o);
    strcat(buf3, "\n");
}

void handleFileOptions() {

    switch (getFileOptions()) {
        case MENU_ITEM_FILE_NEW:
            if (isSaved || warnNotSaved()) {
                if (requestFilename(buf1)) {
                    newTournamentOnCache(buf1);
                }
            }
            break;
        case MENU_ITEM_FILE_OPEN:
            if (isSaved || warnNotSaved()) {
                if (requestFilename(buf1) && !loadTournamentsFromFile(buf1)) {
                    announceFileNotFound();
                }
            }
            break;
        case MENU_ITEM_FILE_SAVE:
            if (saveTournamentToFile()) {
                announceFileSaved();
                isSaved = true;
            } else {
                announceFileNotSaved();
            }
            break;
        case MENU_ITEM_FILE_DELETE:
            if (requestFilename(buf1)) {
                deleteTournamentOnFile(buf1);
                announceOperationSucceed();
            }
            break;
        case MENU_ITEM_FILE_CURRENT:
            resetScreen();
            buf3[0] = '\0';
            strcat(buf3, "最近的运动会记录文件:\n");

            arrayListForEach(previousTournaments, ___function_print_file_name);

            putString(buf3, 3);
            getch();
            break;
        default:
            break;
    }
}

void handleEditOptions() {

    char buf[128];
    isSaved = false;

    switch (getEditOptions()) {
        case MENU_ITEM_EDIT_ADD: ___NEED_ROOT_AUTHENTICATION___
            multipleAdd(getEditWhichOptions(0));
            break;
        case MENU_ITEM_EDIT_EDIT: ___NEED_ROOT_AUTHENTICATION___
            multipleEdit(getEditWhichOptions(1));
            break;
        case MENU_ITEM_EDIT_DELETE: ___NEED_ROOT_AUTHENTICATION___
            multipleDelete(getEditWhichOptions(2));
            break;
        case MENU_ITEM_EDIT_JOIN:
            handleJoin(getJoinOptions());
            break;
        case MENU_ITEM_EDIT_SET_SCORE:
            multipleSetScore(requestInteger(0, cache->matchesCnt - 1));
            break;
        case MENU_ITEM_EDIT_COMMIT: ___NEED_ROOT_AUTHENTICATION___
            if (warnCommitting()) {
                int res = commitOnCache();
                if (res & COMMIT_SUCCEED) {
                    announceOperationSucceed();
                    return;
                } else {
                    buf[0] = '\0';
                    interpretCommitMsg(buf, res);
                    announceCommitFailed(buf);
                }
            }
            break;
        default:
            break;
    }
}

void handleViewOptions() {

    buf3[0] = '\0';

    switch (getViewOptions()) {
        case MENU_ITEM_VIEW_ALL:
            printToString(buf3, cache, printlnTournament);
            break;
        case MENU_ITEM_VIEW_FIELDS:
            printToString(buf3, cache, printFields);
            break;
        case MENU_ITEM_VIEW_TEAMS:
            printToString(buf3, cache, printTeams);
            break;
        case MENU_ITEM_VIEW_MATCHES:
            printToString(buf3, cache, printMatches);
            break;
        case MENU_ITEM_VIEW_ATHLETES:
            printToString(buf3, cache, printAthletes);
            break;
        case MENU_ITEM_VIEW_TOP_MEN:
            printSequenceToString(buf3, cache, topAthletesIdCache, printSortedAthletes);
            break;
        case MENU_ITEM_VIEW_TOP_TEAMS:
            printSequenceToString(buf3, cache, topTeamsIdCache, printSortedTeams);
            break;
        default:
            return;
    }

    resetScreen();
    putString(buf3, DEFAULT_CURSOR_ROW);

    while (getch() == 11) { // ctrl + k
        openTempFileWithNotePad();
    }
}

void handleFindOptions() {
    int id;
    buf3[0] = '\0';
    switch (getFindOptions()) {
        case MENU_ITEM_FIND_FIELD:
            switch (getFindByOptions(0)) {
                case MENU_ITEM_FIND_BY_ID:
                    id = requestInteger(0, cache->fieldCnt - 1);
                    printPartToString(buf3, cache, id, printFieldAt);
                    break;
                case MENU_ITEM_FIND_BY_NAME:
                    requestName(buf1);
                    id = findIdByFieldName(buf1);
                    if (id != WORD_NOT_EXIST) {
                        printPartToString(buf3, cache, id, printFieldAt);
                    } else {
                        announceDataNotExist();
                        return;
                    }
                    break;
                default:
                    break;
            }
            break;
        case MENU_ITEM_FIND_TEAM:
            switch (getFindByOptions(1)) {
                case MENU_ITEM_FIND_BY_ID:
                    id = requestInteger(0, cache->teamsCnt - 1);
                    printPartToString(buf3, cache, id, printTeamsAt);
                    break;
                case MENU_ITEM_FIND_BY_NAME:
                    requestName(buf1);
                    id = findIdByTeamName(buf1);
                    if (id != WORD_NOT_EXIST) {
                        printPartToString(buf3, cache, id, printTeamsAt);
                    } else {
                        announceDataNotExist();
                        return;
                    }
                    break;
                default:
                    return;
            }
            break;
        case MENU_ITEM_FIND_MATCH:
            switch (getFindByOptions(2)) {
                case MENU_ITEM_FIND_BY_ID:
                    id = requestInteger(0, cache->matchesCnt - 1);
                    printPartToString(buf3, cache, id, printMatchAt);
                    break;
                case MENU_ITEM_FIND_BY_NAME:
                    requestName(buf1);
                    id = findIdByMatchName(buf1);
                    if (id != WORD_NOT_EXIST) {
                        printPartToString(buf3, cache, id, printMatchAt);
                    } else {
                        announceDataNotExist();
                        return;
                    }
                    break;
                default:
                    return;
            }
            break;
        case MENU_ITEM_FIND_ATHLETE:
            switch (getFindByOptions(3)) {
                case MENU_ITEM_FIND_BY_ID:
                    id = requestInteger(0, cache->menCnt - 1);
                    printPartToString(buf3, cache, id, printAthletesAt);
                    break;
                case MENU_ITEM_FIND_BY_NAME:
                    requestName(buf1);
                    id = findIdByAthleteName(buf1);
                    if (id != WORD_NOT_EXIST) {
                        printPartToString(buf3, cache, id, printAthletesAt);
                    } else {
                        announceDataNotExist();
                        return;
                    }
                    break;
                default:
                    return;
            }
            break;
        default:
            return;
    }

    resetScreen();
    putString(buf3, DEFAULT_CURSOR_ROW);
    getch();
}

void handleAuthentication() {
    switch (getAuthenticateOption()) {
        case MENU_ITEM_AUTHENTICATE_ROOT:
            if (isRootKeySet()) {
                if (requestPassword(buf1)) {
                    isRootAuthenticated = isRootAccessAccepted(buf1);
                }
            } else {
                isRootAuthenticated = true;
            }
            if (isRootAuthenticated) {
                announceRootAccessAccepted();
            } else {
                announceRootAccessDenied();
            }
            break;
        case MENU_ITEM_AUTHENTICATE_ADMIN:
            if (requestPassword(curAdminKey)) {
                announceAdminAccessSaved();
            }
            break;
        case MENU_ITEM_AUTHORIZE: ___NEED_ROOT_AUTHENTICATION___
            multipleAuthorize();
            break;
        case MENU_ITEM_SET_ROOT_KEY: ___NEED_ROOT_AUTHENTICATION___
            if (requestPassword(buf1) && requestPasswordAgain(buf2)) {
                if (!strcmp(buf1, buf2)) {
                    setRootKey(buf1);
                    announceRootKeySet();
                } else {
                    announceRootKeyDifferent();
                }
            }
            break;
        default:
            return;
    }
}

void handleKey(int which) {

    switch (which) {
        case OPTION_FILE:
            handleFileOptions();
            break;
        case OPTION_EDIT:
            handleEditOptions();
            break;
        case OPTION_VIEW:
            handleViewOptions();
            break;
        case OPTION_FIND:
            handleFindOptions();
            break;
        case OPTION_HELP:
            handleAuthentication();
            break;
        default:
            break;
    }

    resetScreen();
    showEditingFile(cache->name);
}

void quit() {
    closeModel();
    exit(0);
}