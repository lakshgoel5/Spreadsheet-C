#include <check.h>
#include "parser.h"
#include "validate.h"


//--------------------------PARSER------------
START_TEST
        (test_strtocell_const) {
                char* cell = "13";
                int r = 10;
                int c = 10;
                int err = 0;
                Coordinates ans = strtocell(cell, r, c, &err);
                ck_assert_int_eq(ans.row, 13);
                ck_assert_int_eq(ans.col, -1);
                ck_assert_int_eq(err, 0);
        }
END_TEST

START_TEST
        (test_strtocell_const_signed_pos) {
                char* cell = "+13";
                int r = 10;
                int c = 10;
                int err = 0;
                Coordinates ans = strtocell(cell, r, c, &err);
                ck_assert_int_eq(ans.row, 13);
                ck_assert_int_eq(ans.col, -1);
                ck_assert_int_eq(err, 0);
        }
END_TEST

START_TEST
        (test_strtocell_const_signed_neg) {
                char* cell = "-13";
                int r = 10;
                int c = 10;
                int err = 0;
                Coordinates ans = strtocell(cell, r, c, &err);
                ck_assert_int_eq(ans.row, -13);
                ck_assert_int_eq(ans.col, -1);
                ck_assert_int_eq(err, 0);
        }
END_TEST

START_TEST
        (test_strtocell_cell) {
                char* cell = "D3";
                int r = 10;
                int c = 10;
                int err = 0;
                Coordinates ans = strtocell(cell, r, c, &err);
                ck_assert_int_eq(ans.row, 3);
                ck_assert_int_eq(ans.col, 4);
                ck_assert_int_eq(err, 0);
        }
END_TEST

START_TEST
        (test_strtocell_cell_outofbounds) {
                char* cell = "M23";
                int r = 10;
                int c = 10;
                int err = 0;
                Coordinates ans = strtocell(cell, r, c, &err);
                ck_assert_int_eq(ans.row, 23);
                ck_assert_int_eq(ans.col, 13);
                ck_assert_int_eq(err, 1);
        }
END_TEST
//-----------
START_TEST
        (test_charFinder_true) {
                char* str = "A2=3/A2";
                char c = '=';
                ck_assert_int_ne(charFinder(str, c), 0);
        }
END_TEST

START_TEST
        (test_charFinder_false) {
                char* str = "A2=3/A2";
                char c = '$';
                ck_assert_int_eq(charFinder(str, c), 0);
        }
END_TEST
//----------
/*
START_TEST
        (test_parser_assignment) {
                char* cmd = "A1=2";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, -1);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, -1);
                ck_assert_int_eq(ans[2].col, -1);
                ck_assert_int_eq(oper, 0);
        }
END_TEST

START_TEST
        (test_parser_min) {
                char* cmd = "A1=MIN(B2:D3)";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 1);
        }
END_TEST

START_TEST
        (test_parser_max) {
                char* cmd = "A1=MAX(B2:D3)";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 2);
        }
END_TEST

START_TEST
        (test_parser_sum) {
                char* cmd = "A1=SUM(B2:D3)";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 3);
        }
END_TEST

START_TEST
        (test_parser_avg) {
                char* cmd = "A1=AVG(B2:D3)";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 4);
        }
END_TEST

START_TEST
        (test_parser_stdev) {
                char* cmd = "A1=STDEV(B2:D3)";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 5);
        }
END_TEST

START_TEST
        (test_parser_sleep) {
                char* cmd = "A1=SLEEP(B2)";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, -1);
                ck_assert_int_eq(ans[2].col, -1);
                ck_assert_int_eq(oper, 6);
        }
END_TEST

START_TEST
        (test_parser_add) {
                char* cmd = "A1=B2+D3";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 7);
        }
END_TEST

START_TEST
        (test_parser_sub) {
                char* cmd = "A1=B2-D3";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 8);
        }
END_TEST

START_TEST
        (test_parser_mul) {
                char* cmd = "A1=B2*D3";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                ck_assert_int_eq(oper, 9);
        }
END_TEST

START_TEST
         (test_parser_div) {
                char* cmd = "A1=B2/D3";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(ans[1].col, 2);
                ck_assert_int_eq(ans[1].row, 2);
                ck_assert_int_eq(ans[2].row, 3);
                ck_assert_int_eq(ans[2].col, 4);
                free(ans);
                ck_assert_int_eq(oper, 10);
        }
END_TEST
*/
START_TEST
        (test_parser_enable) {
                char* cmd = "enable_output";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(oper, 11);
        }
END_TEST

START_TEST
        (test_parser_disable) {
                char* cmd = "disable_output";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(oper, 12);
        }
END_TEST

START_TEST
        (test_parser_scroll) {
                char* cmd = "scroll_to A1";
                int r = 10;
                int c = 10;
                int oper = 0;
                Coordinates* ans;
                ans = parser(cmd, &oper, r, c);
                ck_assert_int_eq(ans[0].row, 1);
                ck_assert_int_eq(ans[0].col, 1);
                ck_assert_int_eq(oper, 13);
        }
END_TEST
//-------------------------VALIDATE---------------
START_TEST
        (test_val_isNumOrLet) {
                char c = 'a';
                ck_assert_int_eq(isNumOrLet(c), 1);
        }
END_TEST

START_TEST
        (test_val_isNumOrLet_false) {
                char c = '`';
                ck_assert_int_eq(isNumOrLet(c), 0);
        }
END_TEST
//------------
START_TEST
        (test_val_Sym) {
                char c = '-';
                ck_assert_int_eq(isSymbol(c), 1);
        }
END_TEST

START_TEST
        (test_val_sym_false) {
                char c = '$';
                ck_assert_int_eq(isSymbol(c), 0);
        }
END_TEST
//------------
START_TEST
        (test_val_cell) {
                char* str = "AA34";
                ck_assert_int_eq(isCell(str), 1);
        }
END_TEST

START_TEST
        (test_val_cell_false) {
                char* str = "A@D";
                ck_assert_int_eq(isCell(str), 0);
        }
END_TEST
//------------
START_TEST
        (test_val_num) {
                char* str = "5732";
                ck_assert_int_eq(isNum(str), 1);
        }
END_TEST

START_TEST
        (test_val_value) {
                char* str = "A3";
                ck_assert_int_eq(isValue(str), 1);
        }
END_TEST
//-------------
START_TEST
        (test_val_validity) {
                char* cmd = "A2=SLEEP(-3)";
                ck_assert_int_eq(Validity(cmd), 1);
        }
END_TEST

START_TEST
        (test_val_validity_false) {
                char* cmd = "A2=-A3-33";
                ck_assert_int_eq(Validity(cmd), 0);
        }
END_TEST
         
// --------------------------------------SUITES-----------------------------
Suite* ts_parser(void) {
        Suite* s;
        TCase* tc_stc;
        TCase* tc_cf;
        TCase* tc_p;

        s = suite_create("PARSER_TEST_SUITE");

        tc_stc = tcase_create("Testcase for strtocell");
        tcase_add_test(tc_stc, test_strtocell_const);
        tcase_add_test(tc_stc, test_strtocell_const_signed_pos);
        tcase_add_test(tc_stc, test_strtocell_const_signed_neg);
        tcase_add_test(tc_stc, test_strtocell_cell);
        tcase_add_test(tc_stc, test_strtocell_cell_outofbounds);

        tc_cf = tcase_create("Testcase for charFinder");
        tcase_add_test(tc_cf, test_charFinder_true);
        tcase_add_test(tc_cf, test_charFinder_false);

        tc_p = tcase_create("Testcase for parser");
        /*tcase_add_test(tc_p, test_parser_assignment);
        tcase_add_test(tc_p, test_parser_min);
        tcase_add_test(tc_p, test_parser_max);
        tcase_add_test(tc_p, test_parser_sum);
        tcase_add_test(tc_p, test_parser_avg);
        tcase_add_test(tc_p, test_parser_stdev);
        tcase_add_test(tc_p, test_parser_sleep);
        tcase_add_test(tc_p, test_parser_add);
        tcase_add_test(tc_p, test_parser_sub);
        tcase_add_test(tc_p, test_parser_mul);
        tcase_add_test(tc_p, test_parser_div);*/
        tcase_add_test(tc_p, test_parser_enable);
        tcase_add_test(tc_p, test_parser_disable);
        tcase_add_test(tc_p, test_parser_scroll);

        suite_add_tcase(s, tc_p);
        suite_add_tcase(s, tc_cf);
        suite_add_tcase(s, tc_stc);
        return s;
}

Suite* ts_validate(void) {
        Suite* s;
        TCase* tc_NL;
        TCase* tc_sym;
        TCase* tc_cell;
        TCase* tc_NV;
        TCase* tc_vld;

        s = suite_create("VALIDATE_TEST_SUITE");

        tc_NL = tcase_create("Testcase for isNumOrLet");
        tcase_add_test(tc_NL, test_val_isNumOrLet);
        tcase_add_test(tc_NL, test_val_isNumOrLet_false);

        tc_sym = tcase_create("Testcase for isSymbol");
        tcase_add_test(tc_sym, test_val_Sym);
        tcase_add_test(tc_sym, test_val_sym_false);

        tc_cell = tcase_create("Testcase for isCell");
        tcase_add_test(tc_cell, test_val_cell);
        tcase_add_test(tc_cell, test_val_cell_false);

        tc_NV = tcase_create("Testcase for isNum and isValue");
        tcase_add_test(tc_NV, test_val_num);
        tcase_add_test(tc_NV, test_val_value);

        tc_vld = tcase_create("Testcase for Validity");
        tcase_add_test(tc_vld, test_val_validity);
        tcase_add_test(tc_vld, test_val_validity_false);

        suite_add_tcase(s, tc_NL);
        suite_add_tcase(s, tc_sym);
        suite_add_tcase(s, tc_cell);
        suite_add_tcase(s, tc_NV);
        suite_add_tcase(s, tc_vld);

        return s;
}

int main(void) {
        int failures;
        Suite* s1;
        Suite* s2;
        SRunner* sr;

        s1 = ts_parser();
        s2 = ts_validate();
        sr = srunner_create(s1); //creates the suite_runner, starting with s
        srunner_add_suite(sr, s2);

        srunner_run_all(sr, CK_NORMAL);
        failures = srunner_ntests_failed(sr);
        srunner_free(sr);

        return (failures==0) ? 0 : 1;
}
                                               
