#include "minunit.h"
#include <lcthw/bstrlib.h>

char cstr[] = "simple c null-terminated string";
int cstr_len = sizeof(cstr);

char cstr1[] = "another one";
int cstr1_len = sizeof(cstr1);

char *test_bfromcstr()
{	
	bstring bstr = bfromcstr(cstr);

	mu_assert(bstr->slen == cstr_len - 1, "Wrong slen value.");

	mu_assert(bstr->data[bstr->slen - 1] == 'g', "Wrong last but one character.");

	mu_assert(bstr->data[bstr->slen] == '\0', "Wrong last character.");

	bdestroy(bstr);

	return NULL;
}

char *test_blk2bstr()
{
	// test 1
	bstring bstr = blk2bstr(cstr, 6);
	
	mu_assert(bstr->slen == 6, "Wrong slen value.");

	mu_assert(bstr->data[bstr->slen - 1] == 'e', "Wrong last but one character.");

	mu_assert(bstr->data[bstr->slen] == '\0', "Wrong last character.");
	
	// test 2
	bstring bstr1 = blk2bstr(cstr, cstr_len + 10);
	
	mu_assert(bstr1->slen == cstr_len + 10, "Wrong slen value.");

	mu_assert(bstr1->data[bstr1->slen] == '\0', "Wrong last character.");

	bdestroy(bstr);
	bdestroy(bstr1);
	
	return NULL;
}

char *test_bstrcpy()
{
	bstring bstr1 = bfromcstr(cstr);

	bstring bstr2 = bstrcpy(bstr1);

	mu_assert(bstr1->slen == bstr2->slen, "slens aren't equal");

	int i = 0;
	
	for(i = 0; i < bstr1->slen + 1; i++) {
		mu_assert(bstr1->data[i] == bstr2->data[i], "chars aren't equal");
	}

	bdestroy(bstr1);
	bdestroy(bstr2);

	return NULL;
}

char *test_bassign()
{
	return NULL;
}

char *test_bassigncstr()
{
	bstring bstr = bfromcstr(cstr);

	mu_assert(bstr->slen == cstr_len - 1, "Wrong slen value.");

	mu_assert(bstr->data[bstr->slen - 1] == 'g', "Wrong last but one character.");

	mu_assert(bstr->data[bstr->slen] == '\0', "Wrong last character.");

	int res = bassigncstr(bstr, cstr1);
	mu_assert(res == BSTR_OK, "bassigncstr failed");

	mu_assert(bstr->slen == cstr1_len - 1, "Wrong slen value.");

	mu_assert(bstr->data[bstr->slen - 1] == 'e', "Wrong last but one character.");

	mu_assert(bstr->data[bstr->slen] == '\0', "Wrong last character.");

	bdestroy(bstr);
	
	return NULL;
}

char *test_bassignblk()
{
	bstring bstr = bfromcstr(cstr);

	mu_assert(bstr->slen == cstr_len - 1, "Wrong slen value.");

	mu_assert(bstr->data[bstr->slen - 1] == 'g', "Wrong last but one character.");

	mu_assert(bstr->data[bstr->slen] == '\0', "Wrong last character.");

	int res = bassignblk(bstr, cstr1, 7);
	mu_assert(res == BSTR_OK, "bassignblk failed");

	mu_assert(bstr->slen == 7, "Wrong slen value.");

	mu_assert(bstr->data[bstr->slen - 1] == 'r', "Wrong last but one character.");

	mu_assert(bstr->data[bstr->slen] == '\0', "Wrong last character.");

	bdestroy(bstr);

	return NULL;
}

char *test_bdestroy()
{
	bstring bstr = bfromcstr(cstr1);

	int res = bdestroy(bstr);

	mu_assert(res == BSTR_OK, "bdestroy failed");

	return NULL;
}

char *test_bconcat()
{
	bstring bstr1 = bfromcstr(cstr);
	bstring bstr2 = bfromcstr(cstr1);

	mu_assert(bstr1->slen == cstr_len - 1, "Wrong slen");

	int res = bconcat(bstr1, bstr2);
	mu_assert(res == BSTR_OK, "bconcat failed");
	
	mu_assert(bstr1->slen == cstr_len + cstr1_len - 2, "Wrong slen");

	mu_assert(bstr1->data[bstr1->slen - 1] == 'e', "Wrong last but one character.");

	mu_assert(bstr1->data[bstr1->slen] == '\0', "Wrong last character.");

	bdestroy(bstr1);
	bdestroy(bstr2);

	return NULL;
}

char *test_bstricmp()
{
	return NULL;
}

char *test_biseq()
{
	return NULL;
}

char *test_binstr()
{
	return NULL;
}

char *test_bfindreplace()
{
	return NULL;
}

char *test_bsplit()
{
	return NULL;
}

char *test_bformat()
{
	return NULL;
}

char *test_blength()
{
	return NULL;
}

char *test_bdata()
{
	return NULL;
}

char *test_bchar()
{
	return NULL;
}

char *all_tests()
{
	mu_suite_start();

	mu_run_test(test_bfromcstr);
	mu_run_test(test_blk2bstr);
	mu_run_test(test_bstrcpy);
	mu_run_test(test_bassign);
	mu_run_test(test_bassigncstr);
	mu_run_test(test_bassignblk);
	mu_run_test(test_bdestroy);
	mu_run_test(test_bconcat);
	mu_run_test(test_bstricmp);
	mu_run_test(test_biseq);
	mu_run_test(test_binstr);
	mu_run_test(test_bfindreplace);
	mu_run_test(test_bsplit);
	mu_run_test(test_bformat);
	mu_run_test(test_blength);
	mu_run_test(test_bdata);
	mu_run_test(test_bchar);

	return NULL;
}

RUN_TESTS(all_tests);
