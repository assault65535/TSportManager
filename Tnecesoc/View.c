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

    sprintf(buf, "- ��ǰ�򿪵��˶����¼Ϊ: %s", filename);

    putString(buf, 3);
}

void announceOperationSucceed() {
    showMessageBox("- ������� -");
}

void announceFileSaved() {
    showMessageBox("- �˶����¼�ļ�����ɹ� -");
}

void announceFileNotFound() {
    showMessageBox("- ���˶����¼�ļ������� -");
}

void announceFileNotSaved() {
    showMessageBox("- �˶����¼�ļ�����ʧ�� -");
}

void announceOperationIllegal() {
    showMessageBox("- ��ǰ�׶β�֧�ֽ���������� -");
}

void announceDataNotExist() {
    showMessageBox("- �����ڷ��ϲ��������ļ�¼ -");
}

void announceNotAuthorized() {
    showMessageBox("- ��ǰû�н��иò�����Ȩ�� -");
}

void announceRootAccessAccepted() {
    showMessageBox("- ����֤�ɹ� -");
}

void announceAdminAccessSaved() {
    showMessageBox("- ��Ȩ��Կ�ѱ��棬�ֿɽ�����Ȩ�Ĳ��� -");
}

void announceRootAccessDenied() {
    showMessageBox("- ����Կ������֤ʧ�� -");
}

void announceAdminKeyGenerated() {
    showMessageBox("- ��Ȩ��Կ���ɳɹ� -\n�ɲ鿴�˺󵯳�����ʱ������������ϸ��Ϣ");
}

void announceRootKeySet() {
    showMessageBox("- ����Կ�޸ĳɹ� -");
}

void announceRootKeyDifferent() {
    showMessageBox("- �����������벻ͬ, �޸�ʧ�� -");
}

void announceCommitFailed(char *cause) {

    char buf[256];

    sprintf(buf, "- ��ǰ�׶μ�¼��˲�ͨ��, ���ܽ�����һ�׶� -\n\nԭ��: %s\n", cause);

    showMessageBox(buf);
}

void announceEditedKData(int K) {

    char buf[128];

    sprintf(buf, "- �ɹ��ر༭�� %d ����¼ -", K);

    showMessageBox(buf);
}

void announceEditedADataButBDataFailed(int A, int B) {
    char buf[128];
    sprintf(buf, "- �ɹ��ر༭�� %d ����¼, �� %d ����¼�򲻺Ϸ����༭ʧ�� -\n", A, B);
    if (B > 0) {
        strcat(buf, "�ɲ鿴�˺󵯳�����ʱ������������ϸ��Ϣ");
    }
    showMessageBox(buf);
}

bool warnNotSaved() {
    return showAlertDialog("- ��ǰ���ڱ༭���˶����¼��δ���棬�Ƿ����? -");
}

bool warnCommitting() {
    return showAlertDialog("- ���ͨ��������һ�׶κ󽫲��ܳ����޸�, �Ƿ����? -");
}

void toAddSportFieldsPage() {
    system("cls");
    puts("- �����������������");
    puts("- �����ʽ: [������] [���ͬʱ��������]");
    puts("- ���� \"__END_OF_INPUT\" ��������");
    puts("- ����(�����Ϊ 400m_runway �� long_jump_field ����������):");
    puts("- 400m_runway 8");
    puts("- long_jump_field 1");
    puts("- ...");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditSportFieldsPage() {
    system("cls");
    puts("- �������������༭��");
    puts("- �����ʽ: [���� id] [�³�����] [�����ͬʱ��������]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(�޸� id Ϊ 0 �� 1 ����������):");
    puts("- 0 400m_runway 8");
    puts("- 1 long_jump_field 1");
    puts("- -1");
    puts("");
}

void toDeleteSportFieldsPage() {
    system("cls");
    puts("- ������������ɾ����");
    puts("- �����ʽ: [���� id]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(ɾ�� id Ϊ 0, 1 ����������):");
    puts("- 0 1 -1");
    puts("");
}

void toAddTeamsPage() {
    system("cls");
    puts("- �����Ŷ����������");
    puts("- �����ʽ: [�Ŷ���]");
    puts("- ���� \"__END_OF_INPUT\" ��������");
    puts("- ����(�����Ϊ class_1 �� class_2 �������Ŷ�):");
    puts("- class_1");
    puts("- class_2");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditTeamsPage() {
    system("cls");
    puts("- �����Ŷ������༭��");
    puts("- �����ʽ: [�Ŷ� id] [���Ŷ���]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(�޸� id Ϊ 0, 1 �������Ŷ�):");
    puts("- 0 class_1");
    puts("- 1 class_2");
    puts("- -1");
    puts("");
}

void toDeleteTeamsPage() {
    system("cls");
    puts("- �����Ŷ�����ɾ����");
    puts("- �����ʽ: [�Ŷ� id]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(ɾ�� id Ϊ 0, 1 �������Ŷ�):");
    puts("- 0 1 -1");
    puts("");
}

void toAddMatchesPage() {
    system("cls");
    puts("- ������Ŀ���������");
    puts("- �����ʽ: [������] [�ٰ쳡�� id] [��������ʱ��(����, ӦС�� 180)]");
    puts("- [�Ʒ�����(1: ����, ����: ʱ��)] [���μ�����]");
    puts("- [�Ա�����(0:��, 1:�����޶�, 2:Ů���޶�)]");
    puts("- ���� \"__END_OF_INPUT\" ��������");
    puts("- ����(�����Ϊ 400m_running_race �� long_jump ������������Ŀ):");
    puts("- 400m_running_race 0 10 2 32 0");
    puts("- long_jump 1 5 1 16 0");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditMatchesPage() {
    system("cls");
    puts("- ������Ŀ�����༭��");
    puts("- �����ʽ: [���� id] [�±�����] [�³��� id] [�³�������ʱ��(����)]");
    puts("- [�¼Ʒ�����(1: ����, ����: ʱ��)] [�����μ�����]");
    puts("- [�Ա�����(0:��, 1:�����޶�, 2:Ů���޶�)]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(�޸� id Ϊ 0, 1 ������������Ŀ):");
    puts("- 0 400m_running_race 0 10 2 32 0");
    puts("- 1 long_jump 1 5 1 16 0");
    puts("- -1");
    puts("");
}

void toDeleteMatchesPage() {
    system("cls");
    puts("- ������Ŀ����ɾ����");
    puts("- �����ʽ: [���� id]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(ɾ�� id Ϊ 0, 1 ������������Ŀ):");
    puts("- 0 1 -1");
    puts("");
}

void toAddAthletesPage() {
    system("cls");
    puts("- �˶�Ա�������������");
    puts("- �����ʽ: [�˶�Ա��] [�Ա�(1:�� 0:Ů)] [�����Ŷ� id]");
    puts("- ���� \"__END_OF_INPUT\" ��������");
    puts("- ����(�����Ϊ Yuudachi �� Shigure �������˶�Ա�ļ�¼):");
    puts("- Yuudachi 0 0");
    puts("- Shigure 0 1");
    puts("- __END_OF_INPUT");
    puts("");
}

void toEditAthletesPage() {
    system("cls");
    puts("- �˶�Ա���������༭��");
    puts("- �����ʽ: [�˶�Ա id] [���Ա�(1:�� 0:Ů)] [���˶�Ա��] [�������Ŷ� id]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(�޸� id Ϊ 0, 1 ���˶�Ա����):");
    puts("- 0 Yuudachi 0 0");
    puts("- 1 Shigure 0 1");
    puts("- -1");
    puts("");
}

void toDeleteAthletesPage() {
    system("cls");
    puts("- �˶�Ա��¼����ɾ����");
    puts("- �����ʽ: [�˶�Ա id]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(ɾ����ǰ id Ϊ 0, 1 ���˶�Ա):");
    puts("- 0 1 -1");
    puts("");
}

void toJoinMatchPage() {
    system("cls");
    puts("- �˶�Ա����������:");
    puts("- �����ʽ: [�˶�Ա id] [���˶�Ա������˶���Ŀ id] [�μ����(0: ���μ�, ����: �μ�)]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(0 ���˶�Ա�μ� 0 ���˶���Ŀ):");
    puts("- 0 0 1");
    puts("- -1");
    puts("");
}

void toIndividualJoinPage() {
    system("cls");
    puts("- �˶�Ա���˱�����");
    puts("- �����ʽ: [�˶���Ŀ id] [�μ����(0: ���μ�, ����: �μ�)]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(�����μ� 0 ����Ŀ��ȡ������ 1 ����Ŀ):");
    puts("- 0 1");
    puts("- 1 0");
    puts("");
    printf("- ����Ϊ��ǰ���б����������˶�Ա����Ϣ:");
}

void toSetScorePage() {
    system("cls");
    puts("- �����Ƿ���");
    puts("- �����ʽ: [�˶�Ա�ı������] [�˶�Ա�ɼ�(���밴���׼�����, ʱ�䰴���������)]");
    puts("- ���� \"-1\" ��������");
    puts("- ����(�ñ����� 0 ���˶�Ա�ĳɼ�Ϊ 12.87 ��)");
    puts("- 0 12870");
    puts("");
    puts("- ����Ϊ�����˶�����Ϣ:");
}

void toAuthorizePage() {
    system("cls");
    puts("- ������Ȩ��");
    puts("- �����ʽ: [��Ȩ����]");
    puts("- ���� \"__END_OF_INPUT\" ��������");
    puts("- ����(��Ȩһ����Ϊ A ���˶�Ա�޸��䱨����Ŀ��Ȩ��, )");
    puts("-     ͬʱ��Ȩһ�����жԱ��� B �Ƿֵ�Ȩ��):");
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
    "�ļ�(F)", "�༭(E)", "�鿴(V)", "����(S)", "��֤(A)"
};

#define MENU_FILE_ITEMS 6

#define MENU_ITEM_FILE_NEW '0'
#define MENU_ITEM_FILE_OPEN '1'
#define MENU_ITEM_FILE_SAVE '2'
#define MENU_ITEM_FILE_DELETE '3'
#define MENU_ITEM_FILE_CURRENT '4'

Raster fileOptions[] = {
        "|�½� (0)|",
        "|�� (1)|",
        "|���� (2)|",
        "|ɾ�� (3)|",
        "|��� (4)|",
        "|���� (*)|"
};

#define MENU_EDIT_ITEMS 7

#define MENU_ITEM_EDIT_ADD '0'
#define MENU_ITEM_EDIT_EDIT '1'
#define MENU_ITEM_EDIT_DELETE '2'
#define MENU_ITEM_EDIT_JOIN '3'
#define MENU_ITEM_EDIT_SET_SCORE '4'
#define MENU_ITEM_EDIT_COMMIT '5'

Raster editOptions[] = {
        "|���.. (0)|",
        "|�༭.. (1)|",
        "|ɾ��.. (2)|",
        "|����   (3)|",
        "|�Ƿ�   (4)|",
        "|���   (5)|",
        "|����   (*)|"
};

#define MENU_JOIN_ITEMS 3

#define MENU_ITEM_JOIN_INDIVIDUAL '0'
#define MENU_ITEM_JOIN_MULTIPLE '1'

Raster joinOptions[] = {
        "|���˱��� (0)|",
        "|�������� (1)|",
        "|����     (*)|"
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
        "|ȫ��      (0)|",
        "|����      (1)|",
        "|�Ŷ�      (2)|",
        "|����      (3)|",
        "|�˶�Ա    (4)|",
        "|�˶�Ա����(5)|",
        "|�Ŷ�����  (6)|",
        "|����      (*)|"
};

#define MENU_FIND_ITEMS 5

#define MENU_ITEM_FIND_FIELD '0'
#define MENU_ITEM_FIND_TEAM '1'
#define MENU_ITEM_FIND_MATCH '2'
#define MENU_ITEM_FIND_ATHLETE '3'

Raster findOptions[] = {
        "|����  (0)|",
        "|�Ŷ�  (1)|",
        "|����  (2)|",
        "|�˶�Ա(3)|",
        "|����  (*)|"
};

#define MENU_FIND_BY_ITEMS 3

#define MENU_ITEM_FIND_BY_ID '0'
#define MENU_ITEM_FIND_BY_NAME '1'

Raster findByOptions[] = {
        "|ͨ�� id ���� (0)|",
        "|ͨ���������� (1)|",
        "|����         (*)|"
};

#define MENU_AUTHENTICATE_ITEMS 5

#define MENU_ITEM_AUTHENTICATE_ROOT '0'
#define MENU_ITEM_AUTHENTICATE_ADMIN '1'
#define MENU_ITEM_AUTHORIZE '2'
#define MENU_ITEM_SET_ROOT_KEY '3'

Raster authenticateOptions[] = {
        "|����֤    (0)|",
        "|��Ȩ��֤  (1)|",
        "|��Ȩ      (2)|",
        "|�޸�����Կ(3)|",
        "|����      (*)|"
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
    return showTextBox(out, "����һ������С�� 128 �������Լ���.", DEFAULT_CURSOR_ROW);
}

bool requestFilename(char *out) {
    resetScreen();
    return showTextBox(out, "����һ����¼�ļ�������(���� < 128)\n�Լ���.", DEFAULT_CURSOR_ROW);
}

bool requestPassword(char *out) {
    resetScreen();
    return showPasswordTextBox(out, "������֤��Կ�Լ���.");
}

bool requestPasswordAgain(char *out) {
    resetScreen();
    return showPasswordTextBox(out, "�ٴ�������֤��Կ�Լ���.");
}

int requestInteger(int min, int max) {


    char buf[64];
    int ret = 0;
    bool flag;

    do {

        resetScreen();

        sprintf(buf, "����һ��������%d, ������%d���������Լ���.", min, max);
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
            showMessageBox("- �������ݲ��ϻ�����, �벹������ˮƽ -");
        } else {
            break;
        }

    } while (true);

    return ret;
}