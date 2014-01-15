<?php

class Ecomonic
{
    const wsdlUrl = 'https://api.e-conomic.com/secure/api1/EconomicWebservice.asmx?WSDL';

    private $agreementNumber = 844893;
    private $username = "Jør";
    private $password = "kzdfgfm7";
    private $client = null;

    function __construct($agreementNumber = null, $username = null, $password = null)
    {
        if($agreementNumber != null && $password != null && $username != null){
            $this->agreementNumber = $agreementNumber;
            $this->password = $password;
            $this->username = $username;
        }
    }

    /**
     * Connect to e-conomic service.
     * @return SoapClient
     */
    public function connect(){
        $this->client = new SoapClient(self::wsdlUrl, array("trace" => 1, "exceptions" => 1));
        $this->client->Connect(array(
            'agreementNumber' => $this->agreementNumber,
            'userName'        => $this->username,
            'password'        => $this->password
        ));

        return $this->client;
    }

    /**
     * Disconnect from e-conomic service
     * @throws Exception
     */
    public function disconnect(){
        if($this->client != null)
            $this->client->Disconnect();
        else
            throw new Exception("Not connected.");
    }

    /**
     * Get all debtor data.
     * @return mixed
     * @throws Exception
     */
    public function debtor_GetAll(){
        if($this->client != null){
            $debtorHandles = $this->client->Debtor_GetAll()->Debtor_GetAllResult->DebtorHandle;
            $debtorDataObjects = $this->client->Debtor_GetDataArray(array('entityHandles' => $debtorHandles))->Debtor_GetDataArrayResult->DebtorData;
            return $debtorDataObjects;
        }else
            throw new Exception("Not Connected.");
    }

    public function getAgreementNumber(){return $this->agreementNumber;}
    public function getPassword(){return $this->password;}
    public function getUsername(){return $this->username;}
    public function getClient(){return $this->client;}
}
