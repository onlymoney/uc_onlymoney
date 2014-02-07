<?php
/**
 * @file uc_onlymoney.pages.inc
 * Provides functionality of status pages
 */

/**
 * Menu Callback: onlymoney/cancel
 *
 * @return drupal output | redirect 301
 * @see uc_onlymoney_menu
 */
function uc_onlymoney_page_cancel() {
  $cancel_url = variable_get('uc_onlymoney_cancel_url');

  if (!empty($cancel_url)) {
    drupal_goto(variable_get('uc_onlymoney_cancel_url'), array(), 301);
  }

  $output = t('Cancel');

  return $output;
}

/**
 * Menu Callback: onlymoney/status
 *
 * @return string
 * @see uc_onlymoney_menu
 */
function uc_onlymoney_page_status() {

  $watchdog_mode = (bool) variable_get('uc_onlymoney_logging_mode');

  if (!empty($_POST['signature']) && !empty($_POST['operation'])) {
    /**
     * Get input post data
     */

    // object
    $operation = json_decode(urldecode(base64_decode($_POST['operation'])));

    /**
     * Find Order
     */
    $order = uc_order_load($operation->order_id);
    if (empty($order)) {
      if ($watchdog_mode) {
        watchdog(WATCHDOG_ERROR, 'wrong payment order');
      }
      print json_encode(array(
                          'err' => 1,
                          'message' => t('wrong payment order')
                        ));
      exit();
    }

    if (!empty($order->order_total)) {
      if (!empty($order->order_total)) {
        $amount = $order->order_total;
      }

      if (!empty($order->currency)) {
        $currency_code = $order->currency;
      }
    }
    else {
      if ($watchdog_mode) {
        watchdog(WATCHDOG_ERROR, 'Order #' . $order->order_id . 'wrong payment order on merchant site');
      }
      print json_encode(array(
                          'err' => 1,
                          'message' => t('wrong payment order on merchant site')
                        ));
      exit();
    }

    // todo: is not empty: order
    if ($amount != $operation->amount) {
      if ($watchdog_mode) {
        watchdog(WATCHDOG_ERROR,
                 'wrong payment amount (our ' . $amount . ' != '
                 . $operation->amount . ')');
      }
      print json_encode(array(
                          'err' => 1,
                          'message' => t('wrong payment amount')
                        ));
      exit();
    }

    if ($currency_code != $operation->currency) {
      if ($watchdog_mode) {
        watchdog(WATCHDOG_ERROR, 'Order #' . $order->order_id . 'wrong payment currency');
      }
      print json_encode(array(
                          'err' => 1,
                          'message' => t('wrong payment currency')
                        ));
      exit();
    }


    /**
     * Find Signature
     */

    $secret_key = variable_get('uc_onlymoney_secretkey');

    $json = urldecode(base64_decode($_POST['operation']));
    $merchant_signature = sha1($secret_key . $json . $secret_key);
    $signature = urldecode(base64_decode($_POST['signature']));
    if ($merchant_signature != $signature) {
      if ($watchdog_mode) {
        watchdog(WATCHDOG_ERROR,
                 'Order #' . $order->order_id . 'wrong signature (NEW: ' . $merchant_signature . ' SERVER: '
                 . $signature . ')');
      }
      print json_encode(array(
                          'err' => 1,
                          'message' => t('wrong signature')
                        ));
      exit();
    }

  }

  else {
    if ($watchdog_mode) {
      watchdog(WATCHDOG_ERROR, 'wrong signature or order');
    }
    print json_encode(array(
                        'err' => 1,
                        'message' => t('wrong signature or order')
                      ));
    exit();
  }

  if ($watchdog_mode) {
    watchdog(WATCHDOG_INFO, 'Order #' . $order->order_id . ' payment OK');
  }
  print json_encode(array('err' => 0, 'message' => 'payment ok'));
  exit();
}

/**
 * Menu Callback: onlymoney/success
 *
 * authenticate input POST request
 * and close opened transaction
 *
 * @see uc_onlymoney_menu
 */
function uc_onlymoney_page_success() {

  $output = '';
  $watchdog_mode = (bool) variable_get('uc_onlymoney_logging_mode');

  if (!empty($_POST['order_id']) && !empty($_POST['amount'])
    && is_object($order = uc_order_load($_POST['order_id']))
  ) {
    if (!empty($order->order_total)) {
      if (!empty($order->order_total)) {
        $amount = $order->order_total;
      }

      if (!empty($order->currency)) {
        $currency_code = $order->currency;
      }
    }

    if (!empty($amount) && !empty($currency_code) && $amount == $_POST['amount']
      && $currency_code == $_POST['currency']
    ) {
        $order->order_status = 'completed';
        uc_order_save($order);
        uc_cart_empty(uc_cart_get_id());
    }
    else {
      if ($watchdog_mode) {
        watchdog(WATCHDOG_ERROR, 'Wrong amount or currency code');
      }
      drupal_set_message(t('Wrong amount or currency code'), 'error');
    }

    $output = t('Thank you for purchasing. Our manager will contact you shortly');
    $output .= render(uc_order_view_invoice($order));
  }
  else {
    if ($watchdog_mode) {
      watchdog(WATCHDOG_ERROR, 'Wrong order data');
    }
    drupal_set_message(t('Wrong order data'), 'error');
  }

  return $output;
}

/**
 * Menu Callback: onlymoney/fail
 *
 * @return drupal output
 * @see uc_onlymoney_menu
 */
function uc_onlymoney_page_failed() {
  $output = t('Sorry, payment failed. Please, contact site administration');
  return $output;
}
