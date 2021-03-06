<?php

function go1_ui_include_code_mirror($extras = array(), $theme = 'monokai') {
  $path = go1_library('code.mirror', NULL, FALSE);

  drupal_add_js($path . '/lib/codemirror.js');
  drupal_add_css($path . '/lib/codemirror.css');
  drupal_add_css($path . '/theme/'. $theme .'.css');
  foreach ($extras as $extra) {
    drupal_add_js($path . '/' . $extra);
  }
}

/**
 * Add custom assets to /devel/php form to support php syntax.
 * Include CodeMirror assets.
 *
 * @param  array  $extras
 * @param  string $theme
 */
function go1_ui_include_codemirror($extras = array(), $theme = 'monokai') {
  $path = go1_library('code.mirror', NULL, FALSE);

  drupal_add_js($path . '/lib/codemirror.js');
  drupal_add_css($path . '/lib/codemirror.css');
  drupal_add_css($path . '/theme/'. $theme .'.css');
  foreach ($extras as $extra) {
    drupal_add_js($path . '/' . $extra);
  }
}

/**
 * @return string.
 */
function go1_ui_codemirror_submit_shortcut_hint() {
  return 'Use <code>Ctrl+s</code> or <code>Cmd+s</code> to submit the form.';
}

/**
 * Display source code of a file support CodeMirror.
 */
function go1_ui_display_file($form, $form_state, $file, $type = 'unknow') {
  drupal_add_css(drupal_get_path('module', 'go1_ui') . '/misc/css/cm.css');

  switch ($type) {
    case 'twig':
      go1_ui_include_codemirror(array('mode/htmlmixed/htmlmixed.js', 'mode/xml/xml.js', 'addon/mode/overlay.js'));
      drupal_add_js(drupal_get_path('module', 'go1_ui') . '/misc/js/go1.twig.js');
      break;

    case 'yaml':
      go1_ui_include_codemirror(array('mode/yaml/yaml.js'));
      drupal_add_js('(function(){
        Drupal.behaviors.atuiDisplayFile = {
          attach: function(context, settings) {
            var editor = CodeMirror.fromTextArea(document.getElementById("edit-code"), {
              lineNumbers: true
              , viewportMargin: Infinity
              , readOnly: true
              , theme: "monokai"
              , mode: "text/x-yaml"
            });
          }
        };
      })(jQuery);', 'inline');
      break;

    case 'css':
      go1_ui_include_codemirror(array('mode/css/css.js'));
      drupal_add_js('(function(){
        Drupal.behaviors.atuiDisplayFile = {
          attach: function(context, settings) {
            var editor = CodeMirror.fromTextArea(document.getElementById("edit-code"), {
              lineNumbers: true
              , viewportMargin: Infinity
              , readOnly: true
              , theme: "monokai"
              , mode: "text/css"
            });
          }
        };
      })(jQuery);', 'inline');
      break;

    case 'javascript':
      go1_ui_include_codemirror(array('mode/javascript/javascript.js'));
      drupal_add_js('(function(){
        Drupal.behaviors.atuiDisplayFile = {
          attach: function(context, settings) {
            var editor = CodeMirror.fromTextArea(document.getElementById("edit-code"), {
              lineNumbers: true
              , viewportMargin: Infinity
              , readOnly: true
              , theme: "monokai"
              , mode: "text/javascript"
            });
          }
        };
      })(jQuery);', 'inline');
      break;

    case 'php':
      go1_ui_include_codemirror(array('mode/clike/clike.js', 'mode/php/php.js'));
      drupal_add_js(drupal_get_path('module', 'go1_ui') . '/misc/js/devel.php.js');
      break;
  }

  $write = is_writable($file) && isset($_GET['e']);

  $form['code'] = array(
    '#type' => 'textarea',
    '#file' => $file,
    '#default_value' => file_get_contents($file),
    '#disabled' => !$write,
    '#resizable' => FALSE,
  );

  if ($write) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }

  return $form;
}

/**
 * Submit handler for go1_ui_display_file form.
 */
function go1_ui_display_file_submit($form, $form_state) {
  list($file, $content) = array($form['code']['#file'], $form_state['values']['code']);
  $fp = fopen($file, 'w');
  fwrite($fp, $content);
  fclose($fp);
}

function go1_ui_menu_alter__devel(&$menu) {
  $menu['user/%user/devel']['page callback']
    = $menu['user/%user/devel/render']['page callback']
    = $menu['node/%node/devel']['page callback']
    = $menu['node/%node/devel/render']['page callback']
    = $menu['comment/%comment/devel']['page callback']
    = $menu['comment/%comment/devel/render']['page callback']
    = $menu['taxonomy/term/%taxonomy_term/devel']['page callback']
    = $menu['taxonomy/term/%taxonomy_term/devel/render']['page callback']
    = 'go1dr';

  $menu['user/%user/devel']['page arguments']
    = $menu['user/%user/devel/render']['page arguments']
    = $menu['node/%node/devel']['page arguments']
    = $menu['node/%node/devel/render']['page arguments']
    = $menu['comment/%comment/devel']['page arguments']
    = $menu['comment/%comment/devel/render']['page arguments']
    = $menu['taxonomy/term/%taxonomy_term/devel']['page arguments']
    = $menu['taxonomy/term/%taxonomy_term/devel/render']['page arguments']
    = array(1);

  $menu['taxonomy/term/%taxonomy_term/devel']['page arguments']
    = $menu['taxonomy/term/%taxonomy_term/devel/render']['page arguments']
    = array(2);

  if (isset($menu['block/%bean_delta/devel'])) {
    $menu['block/%bean_delta/devel']['page callback'] = 'go1dr';
    $menu['block/%bean_delta/devel/render']['page callback'] = 'go1dr';
    $menu['block/%bean_delta/devel']['page arguments'] = array(1);
    $menu['block/%bean_delta/devel/render']['page arguments'] = array(1);
  }
}
