<?php

require 'vendor/autoload.php';
use Shirtigo\ApiClient\ApiClient;

	class cTShirt_dataset
	{
		
	}
	
	class cTShirt_shop
	{
		/**
		 * @var cTShirt_dataset $tshirt_dataset <br> Enthällt alle Daten die für das T-Shirt-Projekt relevant sind.
		 */
		private $tshirt_dataset;

		/**
		 * @var string 		SHIRTIGO_BASE_URL 	Base Login Credentials for Shirtigo
		 */
		private const SHIRTIGO_BASE_URL = "https://cockpit.shirtigo.de/api/";

		/**
		 * @var string 		SHIRTIGO_BASE_URL 	Base Login Credentials for Shirtigo
		 */
		private const SHIRTIGO_API_TOKEN = "eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6ImI1Mzk2YWZmYWFjMTE1ZDZjOTliMmRmNjVjNGI0ZDQ1NmVhMmQxNjVmYTY2M2QxZWFiOWE2MzU0NWZjZmU4OTE0OTM0ZjY3ZjA2NzU3NmQyIn0.eyJhdWQiOiIxIiwianRpIjoiYjUzOTZhZmZhYWMxMTVkNmM5OWIyZGY2NWM0YjRkNDU2ZWEyZDE2NWZhNjYzZDFlYWI5YTYzNTQ1ZmNmZTg5MTQ5MzRmNjdmMDY3NTc2ZDIiLCJpYXQiOjE1Njk0MjY0MzQsIm5iZiI6MTU2OTQyNjQzNCwiZXhwIjoxODg1MDQ1NjM0LCJzdWIiOiIxNjYwMCIsInNjb3BlcyI6WyJyZWFkLXVzZXIiLCJ3cml0ZS11c2VyIiwicmVhZC1kZXNpZ24iLCJ3cml0ZS1kZXNpZ24iLCJyZWFkLXByb2plY3QiLCJ3cml0ZS1wcm9qZWN0IiwicmVhZC1vcmRlciJdfQ.iYPnQI3J3lSa-i9ynCbpmeVzLgvuBiF8O3I6L_WEkBoj7giy6pB1cS6YcO_IfTaWbn-xAiH5ujpzT5LZMFLPd7N1e-nbfOlSKdLx-XXQEjN-9C3iiHEc7WEO5QcvkJRnOPfQiD3H-lKmnxjQPGTTe1AopF2ZNMEqiu-vRbAtEI4B5s_c1mOrzd-yNLFc7GSiWeeXR2M3M0gxGAJ8SsiDOIwufUVFUVEwmPqzwEJtksr2esFmOk3BhFBZ0Il-ZahHfsm52w8WX5alXXaTWSvwxE1PvaVl5V_KgdpFhmHJOc-NXIaBOdMQMwA5Iu0qCujlxugzaBWpd7odq7_ByqLBfTYwaFFxhNaNftinr70CxXWiFDnDsUcZ5tSqpjoWltzZv6sETSqSfPi1KFFJEgXDhOyze-GmvPUv9O14NsI2f5nfZQH_TKMz8b66uSTONRvvlux_WPfPGYEGqiucPyUJKUajXpDxXGiM0Qr81xsKgwg40v3BydgNErt-64uFqp4_A2e-3nI3mB6_V-7-zfRTgSmENAHB0EEimFuKZXX7aguNjWFUcYoXbai6X3mlG8PYV70TGlqwWbZX4i18Gteg3NJpPakyb_wA8A-ZD87ABrHP0Gy5tNvpQe8zseccRO3kB0KPhRNUrhWcF1V5Om-XPMxhlUFjJp9-p4YQSeZjpDg";
		
		/**
		 * @var ApiClient	$shirtigo_client	Der Engine der die REST
		 */
		private $shirtigo_client;
		
		function __construct()
		{
		}
		
		function __destruct()
		{
			$_SESSION["tshirt_dataset"] = $this->tshirt_dataset;
		}
		
		/**
		 * Login to shirtigo
		 */
		private function login_to_shirtigo()
		{
			$this->shirtigo_client = new ApiClient(cTShirt_shop::SHIRTIGO_API_TOKEN, cTShirt_shop::SHIRTIGO_BASE_URL);
			
		}
		
		public function load_prototype()
		{
		}
		
		public function get_all_product_info()
		{
			
		}
	}//class cTShirt





?>