/*
* (This code has to be used for education purpose only. It has not been tested for production.)
*
* Rounds up the XAH payment.
* Example: 
* 1. If you send 2.3 XAH, the remaining 0.7 XAH will be sent to the specified account.
* 2. If you send 2.6 XAH, the remaining 0.4 XAH will be sent to the specified account.
*
* You'll have to set the **account id**, as hook parameter, to which you want to send the round up amount to.
* To get account id of your raddress, follow this: https://xrpl-hooks.readme.io/reference/util_accid
*
* Key: SA
* Value: account id
*/

#include "hookapi.h"

#define DROPS 1000000

int64_t hook(uint32_t reserved) {

    int64_t tt = otxn_type();
    if (tt != ttPAYMENT)
    {
        rollback(SBUF("XAH Rounding: HookOn field is incorrectly set."), INVALID_TXN);
    }    


    uint8_t alt_account[20];
    uint8_t acc_key[2] = {'S', 'A'};
    int64_t value_len = hook_param(SBUF(alt_account), SBUF(acc_key));

    if(value_len != 20) 
        rollback(SBUF("XAH Rounding: Hooks Parameter Wrong."), __LINE__);

    uint8_t keylet[34];
    if (util_keylet(keylet, 34, KEYLET_ACCOUNT, alt_account, 20, 0, 0, 0, 0) != 34)
        rollback(SBUF("XAH Rounding: Fetching Keylet Failed."), __LINE__);

    if (slot_set(SBUF(keylet), 1) == DOESNT_EXIST)
        rollback(SBUF("XAH Rounding: Account Does Not Exist."), __LINE__);


    etxn_reserve(1);    


    uint8_t hook_acc_id[20];
    hook_account(SBUF(hook_acc_id));

    uint8_t account_field[20];
    otxn_field(SBUF(account_field), sfAccount);

    short equal = 0; BUFFER_EQUAL(equal, hook_acc_id, account_field, 20);

    if(!equal) {
        accept(SBUF("XAH Rounding: Incoming Transaction."), 1);
    }


    unsigned char amount[8];
    int64_t amount_len = otxn_field(SBUF(amount), sfAmount);

    if(amount_len != 8) {
        accept(SBUF("XAH Rounding: Non-XAH Transaction."), 2);
    }

    int64_t otxn_drops = AMOUNT_TO_DROPS(amount);
    int64_t adjustment_amount;

    if(otxn_drops < (int64_t) DROPS) {
        adjustment_amount = (int64_t) DROPS - otxn_drops;
    }

    int64_t fraction_part  = (otxn_drops % (int64_t) DROPS);
    if(fraction_part == 0) {
        accept(SBUF("XAH Rounding: Transaction of rounded number."), 3);
    }

    adjustment_amount = (int64_t) DROPS - fraction_part;


    unsigned char tx[PREPARE_PAYMENT_SIMPLE_SIZE];
    PREPARE_PAYMENT_SIMPLE(tx, adjustment_amount, alt_account, 0, 0);

    uint8_t emithash[32];
    int64_t emit_result = emit(SBUF(emithash), SBUF(tx));


    trace(SBUF("HASH"), SBUF(emithash), 1);
    accept(SBUF("XAH Rounding Successful."), __LINE__);

    _g(1,1);
    return 0;
}