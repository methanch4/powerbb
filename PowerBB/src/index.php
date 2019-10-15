<?php
	/*******************************************************************************
	 * EGS Prüfungs-Übungs-System
	 * 
	 * @author		Siegurt Skoda
	 * @author		Lorena Brachschoss
	 * 
	 * @version		0.01		-	21.05.2019 
	 * 					*	Erste Version
	 */


	/**
	 * Mißt die Zeit die dieses Skript zur Ausführung braucht.
	 * @var float $timer_start
	 */	
	$timer_start = microtime(true);

	/**
	 * Lädt die benötigten Bibliotheken
	 */
	
	// Klassen die das CMS managen
	include "./inc/cSystem.inc";
	include "./inc/cUser.inc";			// Vollumfängliche Benutzerrechteverwaltung
	include "./inc/cForm.inc";			// Formularverwaltung mit gehärteter Sicherheit
	
	
	// Lädt funktionen zur Textverarbeitung
	// (Templates, Sprachen, und Layoutvorlagen)
	include "./inc/template.inc";
	include "./inc/layout.inc";
	include "./inc/cLanguage.inc";
	
	// allgemein nützliche Funktion
	include "./inc/misc.inc";
	
	// Etabliere die Session 
	session_start();
	
	// Lade den Datenbank-Treiber
	include "./inc/cMySQLdrv.inc";
	
	/* cSystem enthällt alle Informationen die sich um die Konfiguration
	 * dieses CMS drehen. Es lädt die Konfigurationsdatenbank und kann
	 * bei Bedarf neue Konfigurationen speichern.  
	 */
	/**
	 * @var cSystem $system					Erzeugt die Systemklasse
	 */
	$system = new cSystem();
	
	if(false)
	{
		/**
		 * Dummydeklaration für die IDE, da diese die Variable $database nicht auflösen kann.
		 * Die eigendliche deklaration der Globalen Variablen $database findet im Konstruktor von
		 * cSystem statt, da dort die konfigurationen für die Datenbanke vorliegen.
		 * @var cDatabase $database
		 */
		$database = new cDatabase("localhost", "dummy", "dummy", "blub");
	}

	/* cUses_manger ist eine mächtige Klasse. Sie Managt die Benutzerinformationen des CMS, 
	 * kümmert sich um das Rechtesystem. Sie überprüft bei jedem Zugriff auf Inhalte ob der 
	 * in der Session hinterlegte User überhaupt berechtigt ist, auf diese Informationen zuzugreifen.
	 * Es werden die für den Benutzer individuellen Eingeschaften geladen und gemanagt.
	 * Um die datenbank zu schonen werden einmal abgerufene Infromationen zu jedem User in der
	 * Session gespeichert. 
	 */
	/**
	 * @var cUser $user 					Erzeugt die Userklasse (oder lädt sie aus der Session)
	 */
	$user = new cUser_manager();	
	
	/* Die cForm_manger Klasse enthällt die Informationen zu den Formularen die in der letzten Seite
	 * gespeichert waren. Die Formularfelder bekommen zufallsgenerierte Namen und sind nur für genau einen
	 * Seitenaufruf gütlig. So wird verhindert das über Formular Hijaking das CMS angegriffen werden kann.
	 * Sollte der aktuelle Skriptaufruf über ein Submit Button erfolgt sein, so wird das Formular automatisch
	 * auf Gültigkeit geprüft, automatisch verarbeitet und in eine dafür vorgesehene Variable gespeichert, 
	 * so das der PreProzessor des CMS die Formulardaten auswerten kann.
	 */	
	/**
	 * @var cForm_manager $form_manager		Erzeugt und Lädt die Verwaltungsdatenbank für Formulare.
	 */
	$form_manager = new cForm_manager();
	
     /**
	 * @var cLanguage $language		Erzeugt und Lädt die Verwaltungsdatenbank für Formulare.
	 */
	$language = new cLanguage();
	
	

	/**
	 * Lädt die CID.
	 * Die CID ist die Contend-ID, in der in der Datenbank des CMS der passende Inhalt zugeordnet werden kann.
	 */	
	$user->load_cid();
	
	/**
	 * body() erzeugt den Inhalt der Webseite.
	 * Dieser wird als (fast) letzter Schritt dann ausgegeben.
	 */	
	echo $language->translate(body());

?>

