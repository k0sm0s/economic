<?php

class Ecomonic
{
    const wsdlUrl = 'https://api.e-conomic.com/secure/api1/EconomicWebservice.asmx?WSDL';

    private $agreementNumber;// = 844893;
    private $username;// = "Jør";
    private $password;// = "kzdfgfm7";
    private $client = null;

    function __construct($agreementNumber, $username, $password)
    {
            $this->agreementNumber = $agreementNumber;
            $this->password = $password;
            $this->username = $username;
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
     * Get all debtor handles.
     * @return array
     * @throws Exception
     */
    public function debtor_GetAll(){
        if($this->client != null){
            $debtorHandles = $this->client->Debtor_GetAll()->Debtor_GetAllResult->DebtorHandle;
            return $debtorHandles;
        }else
            throw new Exception("Not Connected.");
    }

    /**
     * Get all debtor data.
     * @return array
     * @throws Exception
     */
    public function debtor_GetDataArray(){
        if($this->client != null){
            $debtorHandles = $this->debtor_GetAll();
            $debtorDataObjects = $this->client->Debtor_GetDataArray(array('entityHandles' => $debtorHandles))->Debtor_GetDataArrayResult->DebtorData;
            return $debtorDataObjects;
        }else
            throw new Exception("Not Connected.");
    }

    /**
     * Returns handles for all debtor groups.
     * @return array
     */
    public function debtorGroup_GetAll(){
        if($this->client != null){
            $debtorGroupHandles = $this->client->debtorGroup_GetAll()->DebtorGroup_GetAllResult->DebtorGroupHandle;
            return $debtorGroupHandles;
        }else
            throw new Exception("Not Connected.");
    }

    public function debtor_GetData($entityHandle){
        if($this->client != null){
            $debtorDataObject = $this->client->Debtor_GetData(array('entityHandle' => $entityHandle))->Debtor_GetDataResult;
            return $debtorDataObject;
        }else
            throw new Exception("Not Connected.");
    }

    /**
     * Creates a new debtor from a data object.
     * The data object that specifies the properties of the new debtor
     * @param $data
     * @throws Exception
     */
    public function debtor_CreateFromData($data){
        try
        {
            $debtorGroupHandles = $this->debtorGroup_GetAll();
            $firstDebtorGroup = $debtorGroupHandles[0];
            $newDebtorHandle = $this->client->Debtor_Create(array(
                    'number'            => $_POST['debtor_number'],
                    'debtorGroupHandle' => $firstDebtorGroup,
                    'name'              => $_POST['debtor_name'],
                    'vatZone'           => 'EU'))->Debtor_CreateResult;
            $this->client->Debtor_SetAddress(array(
                    'debtorHandle' => $newDebtorHandle,
                    'value'        => $_POST['debtor_address']
                )
            );
        }
        catch(Exception $exception)
        {
            throw $exception;
        }
    }

    public function getAgreementNumber(){return $this->agreementNumber;}
    public function getPassword(){return $this->password;}
    public function getUsername(){return $this->username;}
    public function getClient(){return $this->client;}
}
