<?php

namespace Drupal\islandora\Tuque;

require_once __DIR__ . '/Base.php';

use RepositoryConnection;

/**
 * Class IslandoraRepositoryConnection.
 *
 * @package Drupal\islandora\Tuque
 */
class IslandoraRepositoryConnection extends RepositoryConnection {

  /**
   * Constructor.
   *
   * Invokes parent, but additionally invokes an alter to allow modules to
   * effect the configuration of the connection.
   */
  public function __construct($url = NULL, $username = NULL, $password = NULL) {
    if ($url === NULL) {
      $url = static::FEDORA_URL;
    }
    parent::__construct($url, $username, $password);
    \Drupal::moduleHandler()->alter('islandora_repository_connection_construction', $this);
  }

}
