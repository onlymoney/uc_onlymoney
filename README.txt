
Onlymoney payment system for Ubercart
================================================================================

This module allows you to use Onlymoney payment as a payment method on your
Ubercart store.

Visit https://onlymoney.com/ for more information about this payment system.

Features:
    * quick installation
    * logging mode via watchdogs

Installation
================================================================================

Installation process does not differ from any other drupal module:
https://drupal.org/documentation/install/modules-themes

*Before* starting, make sure that you have already installed Ubercart store and
created an account at https://onlymoney.com/

Log in and visit https://onlymoney.com/merchant. Fill all the necessary fields.
After administrator approves your merchant status you will get your Secret Key
and merchant ID.
Enter them in module settings here:
YOUR_SITE_NAME/admin/store/settings/payment/method/onlymoney

Also don't forget to check Onlymoney in the list of Ubercart payment methods
here: YOUR_SITE_NAME/admin/store/settings/payment

For more detailed instructions visit module help page:
YOUR_SITE_NAME/admin/help/uc_onlymoney
