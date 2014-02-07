<?php
/**
 * @file uc_onlymoney.settings.inc
 */

/**
 * Adds onlymoney settings to the payment method settings form.
 *
 * @see uc_onlymoney_uc_payment_method()
 */
function uc_payment_method_onlymoney($op, &$order, $form = NULL, &$form_state = NULL) {
  switch ($op) {
    case 'settings':
      $form['help'] = array(
        '#markup' => t('<p>Visit help page to get how it works - '
                       . l(t('HELP'), 'admin/help/uc_onlymoney',
                           array('absolute' => TRUE)) . '</p>'),
      );

      $form['uc_onlymoney_merchant_id'] = array(
        '#type' => 'textfield',
        '#title' => t('Your merchant ID'),
        '#description' => t('Your Onlymoney merchant ID.'),
        '#default_value' => variable_get('uc_onlymoney_merchant_id', ''),
        '#size' => 40,
      );

      $form['uc_onlymoney_secretkey'] = array(
        '#type' => 'textfield',
        '#title' => t('Secret Key'),
        '#description' => t('Your Onlymoney secret key.'),
        '#default_value' => variable_get('uc_onlymoney_secretkey', ''),
        '#size' => 40,
      );

      $form['uc_onlymoney_api_url'] = array(
        '#title' => t('Onlymoney API URL'),
        '#type' => 'textfield',
        '#descrtiption' => t(''),
        '#size' => 26,
        '#default_value' => 'https://onlymoney.com/customs/',
        '#disabled' => TRUE,
      );

      $form['uc_onlymoney_cancel_url'] = array(
        '#title' => t('Onlymoney Cancel Page URL'),
        '#type' => 'textfield',
        '#size' => 26,
        '#required' => FALSE,
        '#field_prefix' =>
          url('onlymoney/cancel', array('absolute' => TRUE)) . ' --&gt; ',
        '#default_value' => variable_get('uc_onlymoney_cancel_url'),
        '#description' => t('You can enter the page where user will be redirected to after cancelling the payment <br> You can use alias URL or direct native path: <b>node/1</b> or <b>taxonomy/term/1</b>'),
      );

      $form['uc_onlymoney_logging_mode'] = array(
        '#title' => t('Onlymoney Logging Mode'),
        '#type' => 'checkbox',
        '#default_value' => variable_get('uc_onlymoney_logging_mode'),
        '#description' => t('If checked, module will write logs with watchdog while payment process'),
      );


      $form['#submit'][] = 'uc_onlymoney_settings_submit';

      break;
  }
  return $form;
}

/**
 * Form Submit
 *
 * @see uc_payment_method_onlymoney().
 */
function uc_onlymoney_settings_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('uc_onlymoney_merchant_id', $values['uc_onlymoney_merchant_id']);
  variable_set('uc_onlymoney_secretkey', $values['uc_onlymoney_secretkey']);
  variable_set('uc_onlymoney_api_url', $values['uc_onlymoney_api_url']);
  variable_set('uc_onlymoney_cancel_url', $values['uc_onlymoney_cancel_url']);
  variable_set('uc_onlymoney_logging_mode', $values['uc_onlymoney_logging_mode']);
}
