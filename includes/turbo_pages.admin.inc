<?php

/**
 * @file
 * Administration pages provided by Turbo Pages module.
 */

/**
 * Menu callback for admin/config/search/turbo_pages.
 */
function turbo_pages_form_config($form) {
  global $base_url;
  $path_rss = PATH_TURBO_PAGES_RSS . '/turbo_pages.rss';

  module_load_include('inc', 'turbo_pages', 'includes/turbo_pages.db');
  $content_type_list = _turbo_pages_get_col_from_db('node_type',
    'type',
    'name'
  );

  $text_field_list = _turbo_pages_get_col_from_db('field_config',
    'field_name',
    NULL,
    array('module' => array('text'))
  );

  $image_field_list = _turbo_pages_get_col_from_db('field_config',
    'field_name',
    NULL,
    array('module' => array('image'))
  );

  $form['channel'] = array(
    '#type' => 'fieldset',
    '#title' => t('Channel'),
    '#weight' => 0,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );

  $form['channel']['turbo_pages_rss_url'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS url'),
    '#default_value' => variable_get('turbo_pages_rss_url', 'turbo-pages.rss'),
    '#field_prefix' => $base_url . '/',
  );

  $form['channel']['turbo_pages_channel_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Channel title'),
    '#default_value' => variable_get('turbo_pages_channel_title', NULL),
  );

  $form['channel']['turbo_pages_channel_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Channel description'),
    '#default_value' => variable_get('turbo_pages_channel_description', NULL),
  );

  $form['channel']['turbo_pages_content_types'] = array(
    '#type' => 'checkboxes',
    '#required' => TRUE,
    '#title' => t('Content types'),
    '#options' => $content_type_list,
    '#default_value' => variable_get('turbo_pages_content_types', ''),
    '#description' => t('Binds turbo pages to specified types of materials.'),
  );

  $form['channel']['turbo_pages_content_fields'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#multiple' => TRUE,
    '#title' => t('Content field'),
    '#options' => $text_field_list,
    '#default_value' =>  variable_get('turbo_pages_content_fields', ''),
    '#description' => t('The field where to take content.'),
  );

  $form['channel']['turbo_pages_content_image_field'] = array(
    '#type' => 'select',
    '#required' => FALSE,
    '#multiple' => FALSE,
    '#title' => t('Image field'),
    '#options' => ['none' => t('None')] + $image_field_list,
    '#default_value' =>  variable_get('turbo_pages_content_image_field', ''),
    '#description' => t('The image displayed above the content. The first image is taken, with more than one.'),
  );

  $form['channel']['turbo_pages_username_field'] = array(
    '#type' => 'select',
    '#title' => t('Username field'),
    '#options' => ['none' => t('None')] + $text_field_list,
    '#default_value' =>  variable_get('turbo_pages_username_field', 'none'),
    '#description' => t('The field from which the value of the author will be taken. The default will be <em>$node->name</em>.'),
  );

  $form['creating_rss'] = array(
    '#type' => 'fieldset',
    '#title' => t('Creating RSS file'),
    '#weight' => 1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );

  $form['creating_rss']['turbo_pages_enable_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create by cron'),
    '#default_value' => variable_get('turbo_pages_enable_cron', ''),
  );

  $form['creating_rss']['turbo_pages_rss_update_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Update frequency'),
    '#options' => array(
      86400 => t('Every 1 day'),
      604800 => t('Every 7 day'),
      2592000 => t('Every 30 day'),
      0 => t('Custom')
    ),
    '#default_value' => variable_get('turbo_pages_rss_update_frequency', ''),
    '#states' => array(
      'visible' => array(
        'input[name="turbo_pages_enable_cron"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['creating_rss']['turbo_pages_rss_update_frequency_custom'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom'),
    '#default_value' => variable_get('turbo_pages_rss_update_frequency_custom', ''),
    '#states' => array(
      'visible' => array(
        'select[name="turbo_pages_rss_update_frequency"]' => array(
          'value' => 0,
        ),
        'input[name="turbo_pages_enable_cron"]' => array(
          'checked' => TRUE,
        ),
      ),
      'required' => array(
        'select[name="turbo_pages_rss_update_frequency"]' => array(
          'value' => 0,
        ),
      ),
    ),
  );

  // Create rss file from admin panel.
  $form['creating_rss']['turbo_pages_create_rss'] = array(
    '#type' => 'submit',
    '#value' => t('Create RSS'),
    '#submit' => array('_turbo_pages_create_rss'),
  );

  // Displays information about the created file.
  if (file_exists($path_rss)) {
    $file_created = filemtime($path_rss);
    $form['creating_rss']['turbo_pages_rss_created'] = array(
      '#markup' => '<div class="description">' . t('Created') . ': <b>'
        . format_date($file_created, 'medium') . '</b></div>',
    );
  }

  // Processing added content to file.
  $form['content_processing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content Processing'),
    '#weight' => 2,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => FALSE,
  );

  $form['content_processing']['turbo_pages_items_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Item footer'),
    '#default_value' => variable_get('turbo_pages_items_footer', ''),
    '#description' => t('This text will be added to the basement of each item.'),
  );

  $form['content_processing']['turbo_pages_preg_replaces'] = array(
    '#type' => 'textarea',
    '#title' => t('Preg replaces'),
    '#default_value' => variable_get('turbo_pages_preg_replaces', ''),
    '#description' => t('Enter each regular expression on a new line.'),
  );

  $form['content_processing']['turbo_pages_allowable_tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Allowable tags'),
    '#default_value' => variable_get('turbo_pages_allowable_tags',
      '<p> <h2> <h3> <h4> <h5> <h6> <ul> <ol> <li> <img> <blockquote> <div> <b> <strong> <em>'),
  );

  $form['content_processing']['turbo_pages_content_trim'] = array(
    '#type' => 'checkbox',
    '#title' => t('Trim content to a maximum length'),
    '#default_value' => variable_get('turbo_pages_content_trim', FALSE),
  );

  $form['content_processing']['turbo_pages_content_max_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum length'),
    '#default_value' => variable_get('turbo_pages_content_max_length', ''),
    '#states' => array(
      'visible' => array(
        'input[name="turbo_pages_content_trim"]' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['content_processing']['turbo_pages_rss_minify'] = array(
    '#type' => 'checkbox',
    '#title' => t('Minify RSS'),
    '#default_value' => variable_get('turbo_pages_rss_minify', FALSE),
  );

  $form = system_settings_form($form);

  return $form;
}
