//
// Created by Tnecesoc on 2017/3/4.
//

#ifndef SPORTMANAGER_SPLAY_VIEW_H
#define SPORTMANAGER_SPLAY_VIEW_H

#include "Entity.h"
#include "Utils/Algorithm.h"
#include "Utils/FileStream.h"

#include <stdio.h>
#include <string.h>

#define TEMP_FILE "./temp"
#define TEMP_FILE2 "./temp2"

void openTempFileWithNotePad() {
    openFileWithNotePad(TEMP_FILE);
}

// After-Run Stage
bool topAthleteIds(Tournament *which, ArrayList *res);
bool topTeamIds(Tournament *which, ArrayList *res);

bool ____cmp_athlete_score(const _M a, const _M b) {

    Athlete *ma, *mb;
    ma = arrayListGet(____which->men, a);
    mb = arrayListGet(____which->men, b);

    return ma->score > mb->score;
}

bool topAthleteIds(Tournament *which, ArrayList *res) {

    if (which->status < TOURNAMENT_STATUS_ENDED) {
        return false;
    }

    for (int i = 0; i < which->menCnt; ++i) {
        arrayListAdd(res, &i);
    }

    ____which = which;
    sortWithComparator(res->begin, res->end, ____cmp_athlete_score);
    ____which = NULL;
    return true;
}

bool ____cmp_team_score(const _M a, const _M b) {

    Team *ta, *tb;
    ta = arrayListGet(____which->teams, a);
    tb = arrayListGet(____which->teams, b);

    return ta->score > tb->score;
}

bool topTeams(Tournament *which, ArrayList *res) {

    if (which->status < TOURNAMENT_STATUS_ENDED) {
        return false;
    }

    for (int i = 0; i < which->teamsCnt; ++i) {
        arrayListAdd(res, &i);
    }

    ____which = which;
    sortWithComparator(res->begin, res->end, ____cmp_team_score);
    ____which = NULL;

    return true;
}

void interpretCommitMsg(char *out, int msg) {

    if (msg & COMMIT_NO_FIELD) {
        strcat(out, "没有比赛场地\n");
    }

    if (msg & COMMIT_NO_MATCH) {
        strcat(out, "没有比赛项目\n");
    }

    if (msg & COMMIT_NO_TEAM) {
        strcat(out, "没有比赛团队\n");
    }

    if (msg & COMMIT_NO_ATHLETE) {
        strcat(out, "没有运动员\n");
    }

    if (msg & COMMIT_MATCH_NOT_ALL_FINISHED) {
        strcat(out, "尚有没录入成绩的比赛\n");
    }

    if (msg & COMMIT_MATCH_ENDED) {
        strcat(out, "运动会已经结束\n");
    }

    if (msg & COMMIT_HAVE_CANCELED_MATCHES) {
        strcat(out, "有一些运动项目因参赛人员过少已被取消\n");
    }

}

char *printTime(char *out, double minutes) {
    sprintf(out, "第%d天,%02d:%02d", (int)minutes / 1440 + 1, ((int)minutes % 1440) / 60, (int)minutes % 60);
    return out;
}

char *printSex(char *out, int sex) {

    switch (sex) {
        case 0:
            sprintf(out, "女性");
            break;
        case 1:
            sprintf(out, "男性");
            break;
        default:
            sprintf(out, "未定义");
            break;
    }

    return out;
}

void printSexLimit(FILE *out, int sexLimit) {
    switch (sexLimit) {
        case MATCH_NO_LIMIT:
            break;
        case MATCH_MALE_ONLY:
            fprintf(out, "-   男性限定\n");
            break;
        case MATCH_FEMALE_ONLY:
            fprintf(out, "-   女性限定\n");
            break;
        default:
            break;
    }
}

void printScore(FILE *out, const Tournament *which, int matchId, int score) {

    if (score == MAN_STATUS_NOT_PARTICIPATED) {
        return;
    }

    if (score <= MAN_STATUS_MATCH_NOT_FINISHED) {
        fprintf(out, "暂无成绩");
        return;
    }

    Match *match = arrayListGet(which->matches, matchId);

    if (match->scoreType == MATCH_SCORE_TYPE_DISTANCE) {
        int cm = score;
        fprintf(out, "%.2fm", (double) (cm) / 100);
    } else {
        if (score >= 60000) {
            int minute = score / 60000;
            int second = score % 60000 / 1000;
            int subSecond = score % 1000 / 10;
            fprintf(out, "%dm%02ds%02d", minute, second, subSecond);
        } else {
            int second = score / 1000;
            int subSecond = score % 1000 / 10;
            fprintf(out, "%ds%02d", second, subSecond);
        }
    }
}

void printHiScore(FILE *out, const Tournament *which, int matchId) {

    if (which->status < TOURNAMENT_STATUS_ENDED) {
        fputs("- 当前阶段没有成绩排行榜.", out);
        return;
    }

    Match *match = arrayListGet(which->matches, matchId);

    fprintf(out, match->menCnt > 0 ? "-  成绩排行榜:\n" : "-  已被取消.\n");
    for (int i = 0; i < match->menCnt; ++i) {
        int manId = *(int *) arrayListGet(match->playerIds, i);
        int manScore = *(int *) arrayListGet(match->hiScore, i);
        Athlete *man = arrayListGet(which->men, manId);
        fprintf(out, "-   #%d: %s, ", i, man->name);
        printScore(out, which, matchId, manScore);
        fprintf(out, "\n");
    }
}

void printFieldsId(FILE *out, const Tournament *which) {

    if (which->status < TOURNAMENT_STATUS_CONSTRUCTING) {
        fputs("- 目前阶段没有比赛场地\n", out);
        return;
    }

    fputs(which->fieldCnt <= 0 ? "- 暂无比赛场地\n" : "- 比赛场地:\n", out);
    for (int i = 0; i < which->fieldCnt; ++i) {

        SportField *field = arrayListGet(which->fields, i);

        fprintf(out, "-  #%d: %s\n", i,
                field->name
        );
    }
}

void printTeamsId(FILE *out, const Tournament *which) {

    if (which->status < TOURNAMENT_STATUS_DESIGNING) {
        fputs("- 目前阶段没有比赛团队\n", out);
        return;
    }

    fputs(which->teamsCnt <= 0 ? "- 暂无比赛团队\n" : "- 比赛团队:\n", out);
    for (int i = 0; i < which->teamsCnt; ++i) {

        Team *team = arrayListGet(which->teams, i);

        fprintf(out, "-  #%d: %s\n", i,
                team->name
        );
    }
}

void printMatchesId(FILE *out, const Tournament *which) {

    if (which->status < TOURNAMENT_STATUS_DESIGNING) {
        fputs("- 目前阶段没有比赛项目\n", out);
        return;
    }

    fputs(which->matchesCnt <= 0 ? "- 暂无比赛项目\n" : "- 比赛项目:\n", out);
    for (int i = 0; i < which->matchesCnt; ++i) {

        Match *match = arrayListGet(which->matches, i);

        fprintf(out, "-  #%d: %s\n", i,
                match->name
        );
    }
}

void printAthletesId(FILE *out, const Tournament *which) {
    fputs("- 运动员:\n", out);
    for (int i = 0; i < which->menCnt; ++i) {

        Athlete *man = arrayListGet(which->men, i);

        fprintf(out, "-  #%d: %s\n", i,
                man->name
        );
    }
}

void printFieldAt(FILE *out, const Tournament *which, int id) {

    SportField *field = arrayListGet(which->fields, id);

    if (id < which->fieldCnt) {
        switch (which->status) {
            case TOURNAMENT_STATUS_CONSTRUCTING:
            case TOURNAMENT_STATUS_DESIGNING:
            case TOURNAMENT_STATUS_PENDING:
            case TOURNAMENT_STATUS_RUNNING:
            case TOURNAMENT_STATUS_ENDED:
                fprintf(out, "-  #%d: %s,\n-   最大同时比赛人数:%d\n",
                        id,
                        field->name,
                        field->capacity);
                break;
            default:
                break;
        }
    } else {
        fputs(which->fieldCnt <= id ? "- 该比赛场地不存在." : "", out);
    }
}

void printFields(FILE *out, const Tournament *which) {
    switch (which->status) {
        case TOURNAMENT_STATUS_CONSTRUCTING:
        case TOURNAMENT_STATUS_DESIGNING:
        case TOURNAMENT_STATUS_PENDING:
        case TOURNAMENT_STATUS_RUNNING:
        case TOURNAMENT_STATUS_ENDED:
            fputs(which->fieldCnt > 0 ? "- 比赛场地:\n" : "- 目前还没有比赛场地\n", out);
            for (int i = 0; i < which->fieldCnt; ++i) {
                printFieldAt(out, which, i);
            }
            break;
        default:
            break;
    }
}

void printTeamsAt(FILE *out, const Tournament *which, int id) {

    Team *team = arrayListGet(which->teams, id);

    if (id < which->teamsCnt) {
        switch (which->status) {
            case TOURNAMENT_STATUS_CONSTRUCTING:
                fputs("- 当前阶段没有比赛团队.\n", out);
                break;
            case TOURNAMENT_STATUS_DESIGNING:
            case TOURNAMENT_STATUS_PENDING:
            case TOURNAMENT_STATUS_RUNNING:
                fprintf(out, "-  #%d: %s\n", id,
                        team->name
                );
                break;
            case TOURNAMENT_STATUS_ENDED:
                fprintf(out, "-  #%d: %s, 团队总分: %d\n", id,
                        team->name,
                        team->score
                );
                break;
            default:
                break;
        }
    } else {
        fputs("- 该比赛团队不存在.", out);
    }
}

void printTeams(FILE *out, const Tournament *which) {

    switch (which->status) {
        case TOURNAMENT_STATUS_CONSTRUCTING:
            fputs("- 当前阶段没有比赛团队.\n", out);
            break;
        case TOURNAMENT_STATUS_DESIGNING:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前还没有比赛团队." : "- 比赛团队:");
            for (int i = 0; i < which->teamsCnt; ++i) {
                printTeamsAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_PENDING:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前还没有比赛团队." : "- 比赛团队:");
            for (int i = 0; i < which->teamsCnt; ++i) {
                printTeamsAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_RUNNING:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前还没有比赛团队." : "- 比赛团队:");
            for (int i = 0; i < which->teamsCnt; ++i) {
                printTeamsAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_ENDED:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前还没有比赛团队." : "- 比赛团队:");
            for (int i = 0; i < which->teamsCnt; ++i) {
                printTeamsAt(out, which, i);
            }
            break;
        default:
            break;
    }
}

void printSortedTeams(FILE *out, const Tournament *which, ArrayList *ids) {
    if (which->status < TOURNAMENT_STATUS_ENDED) {
        fprintf(out, "- 当前阶段没有团队排行榜.");
        return;
    }

    fprintf(out, "- 团队排行榜:\n");
    for (int i = 0; i < which->teamsCnt; ++i) {

        int *teamId = arrayListGet(ids, i);
        Team *team = arrayListGet(which->teams, *teamId);

        fprintf(out, "-  #%d: %s, %d\n", i,
                team->name,
                team->score
        );
    }
}

void printAthletesAt(FILE *out, const Tournament *which, int id) {

    char buf[128];

    Athlete *man = arrayListGet(which->men, id);
    Team *team = arrayListGet(which->teams, man->teamId);

    if (which->menCnt > id) {
        switch (which->status) {
            case TOURNAMENT_STATUS_CONSTRUCTING:
            case TOURNAMENT_STATUS_DESIGNING:
                fprintf(out, "- 当前阶段没有运动员纪录.\n");
                break;
            case TOURNAMENT_STATUS_PENDING:
                fprintf(out, "-  #%d: %s, %s\n-   来自 %s 团队,\n-   参加比赛:\n", id,
                        man->name,
                        printSex(buf, man->sex),
                        team->name
                );
                for (int j = 0; j < which->matchesCnt; ++j) {
                    if (*(int *) matrixGet(which->menMatches, j, id) != MAN_STATUS_NOT_PARTICIPATED) {
                        Match *match = arrayListGet(which->matches, j);
                        fprintf(out, "-   %s\n", match->name);
                    }
                }
                break;
            case TOURNAMENT_STATUS_RUNNING:
                fprintf(out, "-  #%d: %s, %s\n-   来自 %s 团队,\n-   参加比赛:\n", id,
                        man->name,
                        printSex(buf, man->sex),
                        team->name
                );
                for (int j = 0; j < which->matchesCnt; ++j) {
                    if (*(int *) matrixGet(which->menMatches, j, id) != MAN_STATUS_NOT_PARTICIPATED) {
                        Match *match = arrayListGet(which->matches, j);
                        fprintf(out, "-     %s\n-     成绩: ", match->name);
                        printScore(out, which, j, *(int *) matrixGet(which->menMatches, j, id));
                        fprintf(out, "\n");
                    }
                }
                break;
            case TOURNAMENT_STATUS_ENDED:
                fprintf(out, "-  #%d: %s, %s\n-   来自 %s 团队,\n-   为团队贡献了 %d 分\n-   参加比赛:\n", id,
                        man->name,
                        printSex(buf, man->sex),
                        team->name,
                        man->score
                );
                for (int j = 0; j < which->matchesCnt; ++j) {
                    if (*(int *) matrixGet(which->menMatches, j, id) != MAN_STATUS_NOT_PARTICIPATED) {
                        Match *match = arrayListGet(which->matches, j);
                        fprintf(out, "-     %s\n-     成绩: ", match->name);
                        printScore(out, which, j, *(int *) matrixGet(which->menMatches, j, id));
                        fprintf(out, "\n");
                    }
                }
                break;
            default:
                break;
        }
    } else {
        fputs("- 该比赛运动员不存在.", out);
    }
}

void printAthletes(FILE *out, const Tournament *which) {
    switch (which->status) {
        case TOURNAMENT_STATUS_CONSTRUCTING:
        case TOURNAMENT_STATUS_DESIGNING:
            fprintf(out, "- 当前阶段没有运动员纪录.\n");
            break;
        case TOURNAMENT_STATUS_PENDING:
            fprintf(out, "%s\n", which->menCnt <= 0 ? "- 目前尚无运动员." : "- 运动员:");
            for (int i = 0; i < which->menCnt; ++i) {
                printAthletesAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_RUNNING:
            fprintf(out, "%s\n", which->menCnt <= 0 ? "- 目前尚无运动员." : "- 运动员:");
            for (int i = 0; i < which->menCnt; ++i) {
                printAthletesAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_ENDED:
            fprintf(out, "%s\n", which->menCnt <= 0 ? "- 目前尚无运动员." : "- 运动员:");
            for (int i = 0; i < which->menCnt; ++i) {
                printAthletesAt(out, which, i);
            }
            break;
        default:
            break;
    }
}

void printSortedAthletes(FILE *out, const Tournament *which, ArrayList *ids) {
    if (which->status < TOURNAMENT_STATUS_ENDED) {
        fprintf(out, "- 当前阶段没有运动员排行榜.");
        return;
    }

    fprintf(out, "- 运动员排行榜:\n");
    for (int i = 0; i < which->menCnt; ++i) {

        int *manId = arrayListGet(ids, i);
        Athlete *man = arrayListGet(which->men, *manId);
        Team *team = arrayListGet(which->teams, man->teamId);

        fprintf(out, "-  #%d: %s,\n-   来自团队 %s,\n-   为团队贡献分数: %d\n", i,
                man->name,
                team->name,
                man->score
        );
    }
}

void printParticipantsOfMatch(FILE *out, const Tournament *which, int id) {

    Match *match = arrayListGet(which->matches, id);

    SportField *field = arrayListGet(which->fields, match->fieldId);

    char buf[128];

    if (which->matchesCnt > id) {
        switch (which->status) {
            case TOURNAMENT_STATUS_CONSTRUCTING:
                fputs("- 当前阶段没有比赛项目.\n", out);
                break;
            case TOURNAMENT_STATUS_DESIGNING:
                fputs("- 当前阶段没有参赛运动员.\n", out);
                break;
            case TOURNAMENT_STATUS_PENDING:
                fprintf(out, "-   参赛运动员:\n");
                for (int j = 0; j < which->menCnt; ++j) {
                    if (*(int *) matrixGet(which->menMatches, id, j) != MAN_STATUS_NOT_PARTICIPATED) {
                        Athlete *man = arrayListGet(which->men, j);
                        fprintf(out, "-    #%d %s\n", j, man->name);
                    }
                }
                break;
            case TOURNAMENT_STATUS_RUNNING:
                if (match->menCnt == MATCH_STATUS_CANCELED) {
                    fprintf(out, "-   已被取消.\n");
                } else {
                    fprintf(out, "-   参赛运动员:\n");

                    for (int i = 0; i < match->menCnt; ++i) {
                        if (i % field->capacity == 0) {
                            double roundStartAt = *(double *) arrayListGet(match->roundStartAt, i / field->capacity);
                            fprintf(out, "-   第 %d 回合(于 %s 开始):\n",
                                    i / field->capacity + 1,
                                    printTime(buf, roundStartAt)
                            );
                        }

                        int manId = *(int *) arrayListGet(match->playerIds, i);
                        Athlete *man = arrayListGet(which->men, manId);

                        fprintf(out, "-    #%d %s(", i % field->capacity, man->name);
                        printScore(out, which, i, *(int *) matrixGet(which->menMatches, id, manId));
                        fprintf(out, ")\n");
                    }
                }
                break;
            case TOURNAMENT_STATUS_ENDED:
                if (match->menCnt == MATCH_STATUS_CANCELED) {
                    fprintf(out, "-   已被取消.\n");
                } else {
                    printHiScore(out, which, id);
                }
                break;
            default:
                break;
        }
    } else {
        fputs("- 该比赛项目不存在", out);
    }
}

void printMatchAt(FILE *out, const Tournament *which, int id) {

    Match *match = arrayListGet(which->matches, id);
    SportField *field = arrayListGet(which->fields, match->fieldId);

    char buf1[64], buf2[64];
    if (which->matchesCnt > id) {
        switch (which->status) {
            case TOURNAMENT_STATUS_CONSTRUCTING:
                fputs("- 当前阶段没有比赛项目.\n", out);
                break;
            case TOURNAMENT_STATUS_DESIGNING:
                fprintf(out, "-  #%d: %s,\n-   场均用时: %.2f 分钟\n-   @%s,\n-   计分类型: %s,\n-   最大参赛人数: %d 人.\n", id,
                        match->name,
                        match->roundTime,
                        field->name,
                        match->scoreType == MATCH_SCORE_TYPE_DISTANCE ? "距离" : "时间",
                        match->maxMen
                );
                printSexLimit(out, match->sexLimit);
                break;
            case TOURNAMENT_STATUS_PENDING:
                fprintf(out, "-  #%d: %s,\n-   @%s,\n-   计分类型: %s,\n-   于 %s 开始,\n-   于 %s 结束\n-   场均用时: %.2f 分钟\n-   最大参赛人数: %d 人,\n-   目前有 %d 人参赛.\n", id,
                        match->name,
                        field->name,
                        match->scoreType == MATCH_SCORE_TYPE_DISTANCE ? "距离" : "时间",
                        printTime(buf1, match->startAt),
                        printTime(buf2, match->endAt),
                        match->roundTime,
                        match->maxMen,
                        matchMenCount(which, id)
                );
                printSexLimit(out, match->sexLimit);
                printParticipantsOfMatch(out, which, id);
                break;
            case TOURNAMENT_STATUS_RUNNING:
                fprintf(out, "-  #%d: %s,\n-   @%s,\n-   计分类型: %s,\n-   于 %s 开始,\n-   于 %s 结束\n-   场均用时: %.2f 分钟\n", id,
                        match->name,
                        field->name,
                        match->scoreType == MATCH_SCORE_TYPE_DISTANCE ? "距离" : "时间",
                        printTime(buf1, match->startAt),
                        printTime(buf2, match->endAt),
                        match->roundTime
                );
                printParticipantsOfMatch(out, which, id);
                break;
            case TOURNAMENT_STATUS_ENDED:
                fprintf(out, "-  #%d: %s,\n-   @%s,\n-   计分类型: %s,\n", id,
                        match->name,
                        field->name,
                        match->scoreType == MATCH_SCORE_TYPE_DISTANCE ? "距离" : "时间"
                );
                printHiScore(out, which, id);
                break;
            default:
                break;
        }
    } else {
        fputs("- 该比赛项目不存在", out);
    }
}

void printMatches(FILE *out, const Tournament *which) {
    switch (which->status) {
        case TOURNAMENT_STATUS_CONSTRUCTING:
            fputs("- 当前阶段没有比赛项目.\n", out);
            break;
        case TOURNAMENT_STATUS_DESIGNING:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前尚无比赛项目." : "- 比赛项目:");
            for (int i = 0; i < which->matchesCnt; ++i) {
                printMatchAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_PENDING:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前尚无比赛项目." : "- 比赛项目:");
            for (int i = 0; i < which->matchesCnt; ++i) {
                printMatchAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_RUNNING:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前尚无比赛项目." : "- 比赛项目:");
            for (int i = 0; i < which->matchesCnt; ++i) {
                printMatchAt(out, which, i);
            }
            break;
        case TOURNAMENT_STATUS_ENDED:
            fprintf(out, "%s\n", which->matchesCnt <= 0 ? "- 目前尚无比赛项目." : "- 比赛项目:");
            for (int i = 0; i < which->matchesCnt; ++i) {
                printMatchAt(out, which, i);
            }
            break;
        default:
            break;
    }
}

void printlnTournament(FILE *out, const Tournament* which) {

    fprintf(out, "- 运动会名: %s\n", which->name);
    fprintf(out, "- 当前状态: ");

    switch (which->status) {
        case TOURNAMENT_STATUS_CONSTRUCTING:
            fprintf(out, "场地准备中\n");
            break;
        case TOURNAMENT_STATUS_DESIGNING:
            fprintf(out, "项目设计中\n");
            break;
        case TOURNAMENT_STATUS_PENDING:
            fprintf(out, "征集报名中\n");
            break;
        case TOURNAMENT_STATUS_RUNNING:
            fprintf(out, "正在运行\n");
            break;
        case TOURNAMENT_STATUS_ENDED:
            fprintf(out, "已经结束\n");
            break;
        default:
            fprintf(out, "未定义\n");
            return;
    }

    fputs("\n", out);
    printFieldsId(out, which);
    fputs("\n", out);
    printTeamsId(out, which);
    fputs("\n", out);
    printMatchesId(out, which);
    fputs("\n", out);
    printAthletesId(out, which);
}

void printPartToString(char *out, const Tournament *which, int id, void(*printer)(FILE *, const Tournament *, int)) {
    FILE *tmp = NULL;

    if ((tmp = fopen(TEMP_FILE, "w")) == NULL) {
        sprintf(out, "ERR_FILE_NOT_AVAILABLE");
        return;
    }

    printer(tmp, which, id);

    fclose(tmp);

    readFileToString(TEMP_FILE, out);
}

void printSequenceToString(char *out, const Tournament *which, ArrayList *id, void(*printer)(FILE *, const Tournament *, ArrayList *)) {
    FILE *tmp = NULL;

    if ((tmp = fopen(TEMP_FILE, "w")) == NULL) {
        sprintf(out, "ERR_FILE_NOT_AVAILABLE");
        return;
    }

    printer(tmp, which, id);

    fclose(tmp);

    readFileToString(TEMP_FILE, out);
}

void printToString(char *out, const Tournament *which, void(*printer)(FILE *, const Tournament *)) {

    FILE *tmp = NULL;

    if ((tmp = fopen(TEMP_FILE, "w")) == NULL) {
        sprintf(out, "ERR_FILE_NOT_AVAILABLE");
        return;
    }

    printer(tmp, which);

    fclose(tmp);

    sprintf(out, "- 按任意键返回.\n");

    readFileToString(TEMP_FILE, out);
}

void saveTournament(FILE *out, const Tournament *which) {

    fprintf(out, "%s\n", which->name);
    fprintf(out, "%d\n", which->status);

    SportField *field;
    Team *team;
    Match *match;
    Athlete *athlete;

    fprintf(out, "%d\n", which->fieldCnt);
    for (int i = 0; i < which->fieldCnt; ++i) {
        field = arrayListGet(which->fields, i);
        fprintf(out, "%s %d\n",
                field->name,
                field->capacity
        );
    }

    fprintf(out, "%d\n", which->teamsCnt);
    for (int i = 0; i < which->teamsCnt; ++i) {
        team = arrayListGet(which->teams, i);
        fprintf(out, "%s %d\n",
                team->name,
                team->score
        );
    }

    fprintf(out, "%d\n", which->matchesCnt);
    for (int i = 0; i < which->matchesCnt; ++i) {
        match = arrayListGet(which->matches, i);
        fprintf(out, "%s %d %d %d %d %d %.2f %.2f %.2f\n",
                match->name,
                match->fieldId,
                match->menCnt,
                match->maxMen,
                match->scoreType,
                match->sexLimit,
                match->roundTime,
                match->startAt,
                match->endAt
        );


        int rounds = match->roundStartAt == NULL ? 0 : arrayListSize(match->roundStartAt);
        fprintf(out, "%d\n", rounds);
        for (int j = 0; j < rounds; ++j) {
            double roundStartAt = *(double *) arrayListGet(match->roundStartAt, i);
            fprintf(out, "%.2f%c",
                    roundStartAt,
                    j == rounds - 1 ? '\n' : ' '
            );
        }

        for (int j = 0; j < match->menCnt; ++j) {
            int playerId = *(int *) arrayListGet(match->playerIds, j);
            fprintf(out, "%d%c",
                    playerId,
                    j == match->menCnt - 1 ? '\n' : ' '
            );
        }

        for (int j = 0; j < match->menCnt; ++j) {
            int hiScore = match->hiScore == NULL ? 0 : *(int *) arrayListGet(match->hiScore, j);
            fprintf(out, "%d%c",
                    hiScore,
                    j == match->menCnt - 1 ? '\n' : ' '
            );
        }
    }

    fprintf(out, "%d\n", which->menCnt);
    for (int i = 0; i < which->menCnt; ++i) {
        athlete = arrayListGet(which->men, i);
        fprintf(out, "%s %d %d %d\n",
                athlete->name,
                athlete->sex,
                athlete->teamId,
                athlete->score
        );
    }

    for (int i = 0; i < which->menCnt; ++i) {
        for (int j = 0; j < which->matchesCnt; ++j) {
            fprintf(out, "%10d%c",
                    *(int *) matrixGet(which->menMatches, j, i),
                    j == which->matchesCnt - 1 ? '\n' : ' '
            );
        }
    }
}

void loadTournament(FILE *in, const Tournament *which) {

    SportField tmpField;
    Team tmpTeam;
    Match tmpMatch;
    Athlete tmpMan;

    fscanf(in, "%s\n", which->name);
    fscanf(in, "%d\n", &which->status);

    fscanf(in, "%d\n", &which->fieldCnt);
    for (int i = 0; i < which->fieldCnt; ++i) {
        fscanf(in, "%s %d\n",
                tmpField.name,
                &tmpField.capacity
        );

        arrayListAdd(which->fields, &tmpField);
    }

    fscanf(in, "%d\n", &which->teamsCnt);
    for (int i = 0; i < which->teamsCnt; ++i) {
        fscanf(in, "%s %d\n",
               tmpTeam.name,
               &tmpTeam.score
        );

        arrayListAdd(which->teams, &tmpTeam);
    }

    tmpMatch.roundStartAt = NULL;
    tmpMatch.playerIds = NULL;
    tmpMatch.hiScore = NULL;

    fscanf(in, "%d\n", &which->matchesCnt);
    for (int i = 0; i < which->matchesCnt; ++i) {
        fscanf(in, "%s %d %d %d %d %d %lf %lf %lf\n",
               tmpMatch.name,
               &tmpMatch.fieldId,
               &tmpMatch.menCnt,
               &tmpMatch.maxMen,
               &tmpMatch.scoreType,
               &tmpMatch.sexLimit,
               &tmpMatch.roundTime,
               &tmpMatch.startAt,
               &tmpMatch.endAt
        );

        int tmpRounds;

        fscanf(in, "%d", &tmpRounds);

        if (tmpRounds > 0) {
            tmpMatch.roundStartAt = newArrayList(sizeof(double));
            double tmpTime;
            for (int j = 0; j < tmpRounds; ++j) {
                fscanf(in, "%lf", &tmpTime);
                arrayListAdd(tmpMatch.roundStartAt, &tmpTime);
            }
        }

        if (tmpMatch.menCnt > 0) {

            tmpMatch.playerIds = newArrayList(sizeof(int));
            tmpMatch.hiScore = newArrayList(sizeof(int));

            for (int j = 0; j < tmpMatch.menCnt; ++j) {
                int playerId;
                fscanf(in, "%d",
                       &playerId
                );

                arrayListAdd(tmpMatch.playerIds, &playerId);
            }

            for (int j = 0; j < tmpMatch.menCnt; ++j) {
                int hiScore;
                fscanf(in, "%d",
                       &hiScore
                );

                arrayListAdd(tmpMatch.hiScore, &hiScore);
            }

        }

        arrayListAdd(which->matches, &tmpMatch);

        if (tmpMatch.roundStartAt != NULL) {
            tmpMatch.roundStartAt = NULL;
        }

        if (tmpMatch.playerIds != NULL) {
            tmpMatch.playerIds = NULL;
        }

        if (tmpMatch.hiScore != NULL) {
            tmpMatch.hiScore = NULL;
        }

    }

    fscanf(in, "%d\n", &which->menCnt);
    for (int i = 0; i < which->menCnt; ++i) {
        fscanf(in, "%s %d %d %d\n",
               tmpMan.name,
               &tmpMan.sex,
               &tmpMan.teamId,
               &tmpMan.score
        );

        arrayListAdd(which->men, &tmpMan);
    }

    for (int i = 0; i < which->matchesCnt; ++i) {
        matrixAddRow(which->menMatches, NULL);
    }

    int mark = MAN_STATUS_NOT_PARTICIPATED;

    for (int i = 0; i < which->menCnt; ++i) {
        matrixAddCell(which->menMatches, &mark);
    }

    for (int i = 0; i < which->menCnt; ++i) {
        for (int j = 0; j < which->matchesCnt; ++j) {
            fscanf(in, "%d", &mark);
            *(int *) matrixGet(which->menMatches, j, i) = mark;
        }
    }
}

#endif //SPORTMANAGER_SPLAY_VIEW_H
