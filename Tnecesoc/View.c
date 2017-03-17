//
// Created by Tnecesoc on 2017/3/5.
//

#include <stdio.h>

#include "Utils/FakeGUI.h"

void initView() {

    reloadBuf();

    char buf[128];

    sprintf(buf, "mode con cols=%d lines=%d", SCREEN_WIDTH, SCREEN_HEIGHT);

    system(buf);

    memset(buf, 0, sizeof(buf));
}

void showEditingFile(char *filename) {

    char buf[128];

    sprintf(buf, "- 当前打开的运动会记录为: %s", filename);

    putString(buf, 3);
}

void announceOperationSucceed() {
    showMessageBox("- 操作完成 -");
}

void announceFileSaved() {
    showMessageBox("- 运动会记录文件保存成功 -");
}

void announceFileNotFound() {
    showMessageBox("- 该运动会记录文件不存在 -");
}

void announceFileNotSaved() {
    showMessageBox("- 运动会记录文件保存失败 -");
}

void announceOperationIllegal() {
    showMessageBox("- 当前阶段不支持进行这项操作 -");
}

void announceDataNotExist() {
    showMessageBox("- 不存在符合查找条件的记录 -");
}

void announceNotAuthorized() {
    showMessageBox("- 当前没有进行该操作的权限 -");
}

void announceRootAccessAccepted() {
    showMessageBox("- 主认证成功 -");
}

void announceAdminAccessSaved() {
    showMessageBox("- 授权密钥已保存，现可进行授权的操作 -");
}

void announceRootAccessDenied() {
    showMessageBox("- 主密钥错误，认证失败 -");
}

void announceAdminKeyGenerated() {
    showMessageBox("- 授权密钥生成成功 -\n可查看此后弹出的临时窗口来查阅详细信息");
}

void announceRootKeySet() {
    showMessageBox("- 主密钥修改成功 -");
}

void announceRootKeyDifferent() {
    showMessageBox("- 两次输入密码不同, 修改失败 -");
}

void announceCommitFailed(char *cause) {

    char buf[256];

    sprintf(buf, "- 当前阶段记录审核不通过, 不能进行下一阶段 -\n\n原因: %s\n", cause);

    showMessageBox(buf);
}

void announceEditedKData(int K) {

    char buf[128];

    sprintf(buf, "- 成功地编辑了 %d 条记录 -", K);

    showMessageBox(buf);
}

void announceEditedADataButBDataFailed(int A, int B) {
    char buf[128];
    sprintf(buf, "- 成功地编辑了 %d 条记录, 有 %d 条记录因不合法而编辑失败 -\n", A, B);
    if (B > 0) {
        strcat(buf, "可查看此后弹出的临时窗口来查阅详细信息");
    }
    showMessageBox(buf);
}

bool warnNotSaved() {
    return showAlertDialog("- 当前正在编辑的运动会记录尚未保存，是否继续? -");
}

bool warnCommitting() {
    return showAlertDialog("- 审核通过进入下一阶段后将不能撤销修改, 是否继续? -");
}

void toAddSportFieldsPage() {
    system("cls");
    puts("- 比赛场地批量添加向导");
    puts("- 输入格式: [场地名] [最大同时比赛人数]");
    puts("- 输入 \"__END_OF_INPUT\" 结束输入");
    puts("- 范例(添加名为 400m_runway 和 long_jump_field 的两个场地):");
    puts("- 400m_runway 8");
    puts("- long_jump_field 1");
    puts("- ...");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditSportFieldsPage() {
    system("cls");
    puts("- 比赛场地批量编辑向导");
    puts("- 输入格式: [场地 id] [新场地名] [新最大同时比赛人数]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(修改 id 为 0 和 1 的两个场地):");
    puts("- 0 400m_runway 8");
    puts("- 1 long_jump_field 1");
    puts("- -1");
    puts("");
}

void toDeleteSportFieldsPage() {
    system("cls");
    puts("- 比赛场地批量删除向导");
    puts("- 输入格式: [场地 id]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(删除 id 为 0, 1 的两个场地):");
    puts("- 0 1 -1");
    puts("");
}

void toAddTeamsPage() {
    system("cls");
    puts("- 比赛团队批量添加向导");
    puts("- 输入格式: [团队名]");
    puts("- 输入 \"__END_OF_INPUT\" 结束输入");
    puts("- 范例(添加名为 class_1 和 class_2 的两个团队):");
    puts("- class_1");
    puts("- class_2");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditTeamsPage() {
    system("cls");
    puts("- 比赛团队批量编辑向导");
    puts("- 输入格式: [团队 id] [新团队名]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(修改 id 为 0, 1 的两个团队):");
    puts("- 0 class_1");
    puts("- 1 class_2");
    puts("- -1");
    puts("");
}

void toDeleteTeamsPage() {
    system("cls");
    puts("- 比赛团队批量删除向导");
    puts("- 输入格式: [团队 id]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(删除 id 为 0, 1 的两个团队):");
    puts("- 0 1 -1");
    puts("");
}

void toAddMatchesPage() {
    system("cls");
    puts("- 比赛项目批量添加向导");
    puts("- 输入格式: [比赛名] [举办场地 id] [场均比赛时间(分钟, 应小于 180)]");
    puts("- [计分类型(1: 距离, 其他: 时间)] [最大参加人数]");
    puts("- [性别限制(0:无, 1:男性限定, 2:女性限定)]");
    puts("- 输入 \"__END_OF_INPUT\" 结束输入");
    puts("- 范例(添加名为 400m_running_race 和 long_jump 的两个比赛项目):");
    puts("- 400m_running_race 0 10 2 32 0");
    puts("- long_jump 1 5 1 16 0");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditMatchesPage() {
    system("cls");
    puts("- 比赛项目批量编辑向导");
    puts("- 输入格式: [比赛 id] [新比赛名] [新场地 id] [新场均比赛时间(分钟)]");
    puts("- [新计分类型(1: 距离, 其他: 时间)] [新最大参加人数]");
    puts("- [性别限制(0:无, 1:男性限定, 2:女性限定)]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(修改 id 为 0, 1 的两个比赛项目):");
    puts("- 0 400m_running_race 0 10 2 32 0");
    puts("- 1 long_jump 1 5 1 16 0");
    puts("- -1");
    puts("");
}

void toDeleteMatchesPage() {
    system("cls");
    puts("- 比赛项目批量删除向导");
    puts("- 输入格式: [比赛 id]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(删除 id 为 0, 1 的两个比赛项目):");
    puts("- 0 1 -1");
    puts("");
}

void toAddAthletesPage() {
    system("cls");
    puts("- 运动员资料批量添加向导");
    puts("- 输入格式: [运动员名] [性别(1:男 0:女)] [所属团队 id]");
    puts("- 输入 \"__END_OF_INPUT\" 结束输入");
    puts("- 范例(添加名为 Yuudachi 和 Shigure 的两名运动员的记录):");
    puts("- Yuudachi 0 0");
    puts("- Shigure 0 1");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditAthletesPage() {
    system("cls");
    puts("- 运动员资料批量编辑向导");
    puts("- 输入格式: [运动员 id] [新性别(1:男 0:女)] [新运动员名] [新所属团队 id]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(修改 id 为 0, 1 的运动员资料):");
    puts("- 0 Yuudachi 0 0");
    puts("- 1 Shigure 0 1");
    puts("- -1");
    puts("");
}

void toDeleteAthletesPage() {
    system("cls");
    puts("- 运动员记录批量删除向导");
    puts("- 输入格式: [运动员 id]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(删除当前 id 为 0, 1 的运动员):");
    puts("- 0 1 -1");
    puts("");
}

void toJoinMatchPage() {
    system("cls");
    puts("- 运动员批量报名向导:");
    puts("- 输入格式: [运动员 id] [该运动员参与的运动项目 id] [参加与否(0: 不参加, 其他: 参加)]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(0 号运动员参加 0 号运动项目):");
    puts("- 0 0 1");
    puts("- -1");
    puts("");
}

void toIndividualJoinPage() {
    system("cls");
    puts("- 运动员个人报名向导");
    puts("- 输入格式: [运动项目 id] [参加与否(0: 不参加, 其他: 参加)]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(报名参加 0 号项目并取消报名 1 号项目):");
    puts("- 0 1");
    puts("- 1 0");
    puts("");
    printf("- 以下为当前进行报名操作的运动员的信息:");
}

void toSetScorePage() {
    system("cls");
    puts("- 批量记分向导");
    puts("- 输入格式: [运动员的比赛编号] [运动员成绩(距离按厘米计整数, 时间按毫秒计整数)]");
    puts("- 输入 \"-1\" 结束输入");
    puts("- 范例(该比赛的 0 号运动员的成绩为 12.87 秒)");
    puts("- 0 12870");
    puts("");
    puts("- 以下为该项运动的信息:");
}

void toAuthorizePage() {
    system("cls");
    puts("- 批量授权向导");
    puts("- 输入格式: [授权名义]");
    puts("- 输入 \"__END_OF_INPUT\" 结束输入");
    puts("- 范例(授权一个名为 A 的运动员修改其报名项目的权限, )");
    puts("-     同时授权一个裁判对比赛 B 记分的权限):");
    puts("- A");
    puts("- B");
    puts("");
}

#define TOOLBAR_ITEMS 5

#define OPTION_FILE 'F'
#define OPTION_EDIT 'E'
#define OPTION_VIEW 'V'
#define OPTION_FIND 'S'
#define OPTION_HELP 'A'

Raster toolBar[] = {
    "文件(F)", "编辑(E)", "查看(V)", "搜索(S)", "认证(A)"
};

#define MENU_FILE_ITEMS 6

#define MENU_ITEM_FILE_NEW '0'
#define MENU_ITEM_FILE_OPEN '1'
#define MENU_ITEM_FILE_SAVE '2'
#define MENU_ITEM_FILE_DELETE '3'
#define MENU_ITEM_FILE_CURRENT '4'

Raster fileOptions[] = {
        "|新建 (0)|",
        "|打开 (1)|",
        "|保存 (2)|",
        "|删除 (3)|",
        "|最近 (4)|",
        "|返回 (*)|"
};

#define MENU_EDIT_ITEMS 7

#define MENU_ITEM_EDIT_ADD '0'
#define MENU_ITEM_EDIT_EDIT '1'
#define MENU_ITEM_EDIT_DELETE '2'
#define MENU_ITEM_EDIT_JOIN '3'
#define MENU_ITEM_EDIT_SET_SCORE '4'
#define MENU_ITEM_EDIT_COMMIT '5'

Raster editOptions[] = {
        "|添加.. (0)|",
        "|编辑.. (1)|",
        "|删除.. (2)|",
        "|报名   (3)|",
        "|记分   (4)|",
        "|审核   (5)|",
        "|返回   (*)|"
};

#define MENU_JOIN_ITEMS 3

#define MENU_ITEM_JOIN_INDIVIDUAL '0'
#define MENU_ITEM_JOIN_MULTIPLE '1'

Raster joinOptions[] = {
        "|个人报名 (0)|",
        "|批量报名 (1)|",
        "|返回     (*)|"
};

#define MENU_VIEW_ITEMS 8

#define MENU_ITEM_VIEW_ALL '0'
#define MENU_ITEM_VIEW_FIELDS '1'
#define MENU_ITEM_VIEW_TEAMS '2'
#define MENU_ITEM_VIEW_MATCHES '3'
#define MENU_ITEM_VIEW_ATHLETES '4'
#define MENU_ITEM_VIEW_TOP_MEN '5'
#define MENU_ITEM_VIEW_TOP_TEAMS '6'

Raster viewOptions[] = {
        "|全部      (0)|",
        "|场地      (1)|",
        "|团队      (2)|",
        "|比赛      (3)|",
        "|运动员    (4)|",
        "|运动员排行(5)|",
        "|团队排行  (6)|",
        "|返回      (*)|"
};

#define MENU_FIND_ITEMS 5

#define MENU_ITEM_FIND_FIELD '0'
#define MENU_ITEM_FIND_TEAM '1'
#define MENU_ITEM_FIND_MATCH '2'
#define MENU_ITEM_FIND_ATHLETE '3'

Raster findOptions[] = {
        "|场地  (0)|",
        "|团队  (1)|",
        "|比赛  (2)|",
        "|运动员(3)|",
        "|返回  (*)|"
};

#define MENU_FIND_BY_ITEMS 3

#define MENU_ITEM_FIND_BY_ID '0'
#define MENU_ITEM_FIND_BY_NAME '1'

Raster findByOptions[] = {
        "|通过 id 搜索 (0)|",
        "|通过名称搜索 (1)|",
        "|返回         (*)|"
};

#define MENU_AUTHENTICATE_ITEMS 5

#define MENU_ITEM_AUTHENTICATE_ROOT '0'
#define MENU_ITEM_AUTHENTICATE_ADMIN '1'
#define MENU_ITEM_AUTHORIZE '2'
#define MENU_ITEM_SET_ROOT_KEY '3'

Raster authenticateOptions[] = {
        "|主认证    (0)|",
        "|授权认证  (1)|",
        "|授权      (2)|",
        "|修改主密钥(3)|",
        "|返回      (*)|"
};

#define MENU_ITEM_MARGIN ((SCREEN_WIDTH - TOOLBAR_ITEMS * strlen(toolBar[0])) / (TOOLBAR_ITEMS - 1))

void showToolbar() {

    char blank[128];

    for (int i = 0; i < MENU_ITEM_MARGIN; ++i) {
        blank[i] = ' ';
    }
    blank[MENU_ITEM_MARGIN] = '\0';

    sprintf(screenBuf[0], "%s%s%s%s%s%s%s%s%s",
            toolBar[0], blank, toolBar[1], blank, toolBar[2], blank, toolBar[3], blank, toolBar[4]
    );

    for (int i = 0; i < SCREEN_WIDTH; ++i) {
        screenBuf[1][i] = '=';
    }

    refreshScreen();
}

void resetScreen() {

    reloadBuf();

    showToolbar();

}

int getFileOptions() {

    uploadCharBlock(fileOptions, MENU_FILE_ITEMS, 2, 0);

    refreshScreen();

    return getch();
}

int getEditOptions() {

    uploadCharBlock(editOptions, MENU_EDIT_ITEMS, 2,
                    (MENU_ITEM_MARGIN + strlen(toolBar[0])));

    refreshScreen();

    return getch();
}

int getJoinOptions() {

    uploadCharBlock(joinOptions, MENU_JOIN_ITEMS, 2 + (MENU_ITEM_EDIT_JOIN - '0'),
                    (MENU_ITEM_MARGIN + strlen(toolBar[0]) + strlen(editOptions[0])));

    refreshScreen();

    return getch();
}

int getViewOptions() {

    uploadCharBlock(viewOptions, MENU_VIEW_ITEMS, 2,
                    2 * (strlen(toolBar[0]) + MENU_ITEM_MARGIN));

    refreshScreen();

    return getch();
}

int getFindOptions() {

    uploadCharBlock(findOptions, MENU_FIND_ITEMS, 2,
                    3 * (strlen(toolBar[0]) + MENU_ITEM_MARGIN));

    refreshScreen();

    return getch();
}

int getEditWhichOptions(int at) {

    uploadCharBlock(findOptions, MENU_FIND_ITEMS, 2 + at,
                    (strlen(toolBar[0]) + MENU_ITEM_MARGIN + strlen(editOptions[0])));

    refreshScreen();

    return getch();
}

int getFindByOptions(int at) {

    uploadCharBlock(findByOptions, MENU_FIND_BY_ITEMS, 2 + at,
                    3 * (MENU_ITEM_MARGIN + strlen(toolBar[0])) + strlen(findOptions[0]));

    refreshScreen();

    return getch();
}

int getAuthenticateOption() {

    uploadCharBlock(authenticateOptions, MENU_AUTHENTICATE_ITEMS, 2, SCREEN_WIDTH - 1 - strlen(authenticateOptions[0]));

    refreshScreen();

    return getch();
}

bool requestName(char *out) {
    resetScreen();
    return showTextBox(out, "输入一个长度小于 128 的名称以继续.", DEFAULT_CURSOR_ROW);
}

bool requestFilename(char *out) {
    resetScreen();
    return showTextBox(out, "输入一个记录文件的名称(长度 < 128)\n以继续.", DEFAULT_CURSOR_ROW);
}

bool requestPassword(char *out) {
    resetScreen();
    return showPasswordTextBox(out, "输入认证密钥以继续.");
}

bool requestPasswordAgain(char *out) {
    resetScreen();
    return showPasswordTextBox(out, "再次输入认证密钥以继续.");
}

int requestInteger(int min, int max) {


    char buf[64];
    int ret = 0;
    bool flag;

    do {

        resetScreen();

        sprintf(buf, "输入一个不低于%d, 不高于%d的正整数以继续.", min, max);
        showTextBox(buf, buf, DEFAULT_CURSOR_ROW);

        if (buf[0] >= '0' && buf[0] <= '9') {
            sscanf(buf, "%d", &ret);
            flag = ret < min || ret > max;
        } else if ((buf[0] == '+' || buf[0] == '-') && (buf[1] >= '0' && buf[1] <= '9')) {
            sscanf(buf, "%d", &ret);
            flag = ret < min || ret > max;
        } else {
            flag = true;
        }

        if (flag) {
            showMessageBox("- 输入数据不合基本法, 请补充姿势水平 -");
        } else {
            break;
        }

    } while (true);

    return ret;
}