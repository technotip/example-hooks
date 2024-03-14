## What does the hooks do?

Rounds up the XAH payment.

Example:

1. If you send 2.3 XAH, the remaining 0.7 XAH will be sent to the specified account.
2. If you send 2.6 XAH, the remaining 0.4 XAH will be sent to the specified account.

You'll have to set the **account id**, as hook parameter, to which you want to send the round up amount to.

## Installing the hook

Hooks should be installed with an raddress(in hex format) as a parameter.

**SA** is the key. **SA** stands for Savings Account.
**value** should be the 20 byte account ID(hex representation of any of your raddress).

Follow this code to get the 20 bye account ID of your raddress: [util_accid](https://xrpl-hooks.readme.io/reference/util_accid)