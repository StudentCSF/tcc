#include <TCC/bank/TCC_banks.h>
#include <TCC/bank/TCC_sber.h>
#include <TCC/bank/TCC_tbank.h>
#include <TCC/bank/TCC_raiffeisen.h>
#include <check.h>
#include <stdlib.h>

static struct {
    const TCC_bank_api* sber;
    const TCC_bank_api* tbank;
    const TCC_bank_api* raif;
} test_ctx;

static void setup(void) {
    test_ctx.sber = TCC_sber_InitApi();
    test_ctx.tbank = TCC_tbank_InitApi(&(TCC_tbank_auth_context){.digital_signature = "TBS-TEST"});
    test_ctx.raif = TCC_raiffeisen_InitApi("RAIF-TEST");
    
    TCC_RegisterBank("sber", test_ctx.sber, NULL);
    TCC_RegisterBank("tbank", test_ctx.tbank, NULL);
    TCC_RegisterBank("raif", test_ctx.raif, NULL);
}

static void teardown(void) {
    TCC_CleanupAll();
}

START_TEST(test_register_bank) {
    TCC_bank_api_status status = TCC_RegisterBank("test_bank", test_ctx.sber, NULL);
    ck_assert_int_eq(status, TCC_bank_api_ok);
}
END_TEST

START_TEST(test_unregister_bank) {
    TCC_bank_api_status status = TCC_UnregisterBank("sber");
    ck_assert_int_eq(status, TCC_bank_api_ok);
}
END_TEST

START_TEST(test_set_active_bank) {
    TCC_bank_api_status status = TCC_SetActiveBank("tbank");
    ck_assert_int_eq(status, TCC_bank_api_ok);
}
END_TEST

START_TEST(test_get_active_bank_name) {
    TCC_SetActiveBank("raif");
    const char* name = TCC_GetActiveBankName();
    ck_assert_str_eq(name, "raif");
}
END_TEST

START_TEST(test_get_transactions) {
    TCC_SetActiveBank("sber");
    TCC_bank_transaction* transactions;
    size_t count;
    
    TCC_bank_api_status status = TCC_GetTransactions("sber_123", &transactions, &count);
    ck_assert_int_eq(status, TCC_bank_api_ok);
    ck_assert_uint_eq(count, 3);
    ck_assert_str_eq(transactions[0].currency, "RUB");
}
END_TEST

START_TEST(test_get_inn) {
    TCC_SetActiveBank("tbank");
    char inn[13];
    
    TCC_bank_api_status status = TCC_GetInn("tbank_456", inn, sizeof(inn));
    ck_assert_int_eq(status, TCC_bank_api_ok);
    ck_assert_str_eq(inn, "7710140679");
}
END_TEST

START_TEST(test_get_interest_rates) {
    TCC_SetActiveBank("raif");
    TCC_loan_interest_rate* rates;
    size_t count;
    
    TCC_bank_api_status status = TCC_GetInterestRates("raif_789", &rates, &count);
    ck_assert_int_eq(status, TCC_bank_api_ok);
    ck_assert_uint_eq(count, 2);
    ck_assert_str_eq(rates[0].loan_type, "mortgage");
}
END_TEST

START_TEST(test_get_card_credits) {
    TCC_SetActiveBank("sber");
    TCC_card_credit* credits;
    size_t count;
    
    TCC_bank_api_status status = TCC_GetCardCredits("sber_123", &credits, &count);
    ck_assert_int_eq(status, TCC_bank_api_ok);
    ck_assert_uint_eq(count, 2);
    ck_assert_str_eq(credits[0].source, "employer");
}
END_TEST

START_TEST(test_cleanup_all) {
    TCC_CleanupAll();
    ck_assert_ptr_eq(TCC_GetActiveBankName(), NULL);
}
END_TEST

Suite* banks_suite(void) {
    Suite* s = suite_create("Banks");
    TCase* tc = tcase_create("Core");
    
    tcase_add_checked_fixture(tc, setup, teardown);
    tcase_add_test(tc, test_register_bank);
    tcase_add_test(tc, test_unregister_bank);
    tcase_add_test(tc, test_set_active_bank);
    tcase_add_test(tc, test_get_active_bank_name);
    tcase_add_test(tc, test_get_transactions);
    tcase_add_test(tc, test_get_inn);
    tcase_add_test(tc, test_get_interest_rates);
    tcase_add_test(tc, test_get_card_credits);
    tcase_add_test(tc, test_cleanup_all);
    
    suite_add_tcase(s, tc);
    return s;
}

int main(void) {
    int number_failed;
    Suite* s = banks_suite();
    SRunner* sr = srunner_create(s);
    
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}