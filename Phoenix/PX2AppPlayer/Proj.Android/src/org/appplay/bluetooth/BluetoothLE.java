package org.appplay.bluetooth;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.Vector;

import org.appplay.bluetooth.BluetoothListener.AutoConnectionListener;
import org.appplay.bluetooth.BluetoothListener.BluetoothConnectionListener;
import org.appplay.lib.AppPlayBaseActivity;
import org.appplay.lib.AppPlayNatives;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

public class BluetoothLE extends Service
{	
	private Context mContext;
	private BluetoothAdapter mBluetoothAdapter = null;
	private BluetoothGatt mBluetoothGatt = null;
	
    private BluetoothListener.BluetoothStateListener mBluetoothStateListener = null;
    private BluetoothListener.OnDataReceivedListener mDataReceivedListener = null;
    private BluetoothListener.BluetoothConnectionListener mBluetoothConnectionListener = null;
    private BluetoothListener.AutoConnectionListener mAutoConnectionListener = null;
	
	private boolean mScanning;
    private Handler mHandler;
    private ArrayList<BluetoothDevice> mLeDevices = new ArrayList<BluetoothDevice>();
	
	private String mBluetoothDeviceAddress;
	
	private int mConnectionState = BluetoothState.STATE_NONE;
	
	private String TAG = "BluetoothLE";
    private static final long SCAN_PERIOD = 10000;
	
    private ArrayList<byte[]> mSendList = new ArrayList<byte[]>();
	public boolean mIsCanSend = true;
    private static int SendItemWirteBytes = 0;
    private static int CurSendItemBytes = 0;
    private static final String SendTag = "Data";
    private static final int BLE_SEND_OK = 1;
    
    private BluetoothAdapter.LeScanCallback mLeScanCallback = null;
    private BluetoothGattCallback mGattCallback = null;
    
    public BluetoothLE(Context context)
    {
    	mContext = context;
    	
    	// API����������android4.3     
    	mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    	
    	if (null != mBluetoothAdapter)
    	{  		
    	}
    }
    
    public void Initlize()
    {
    	mHandler = new Handler();	

		// listener
		SetBluetoothConnectionListener(new BluetoothConnectionListener() {
			public void onDeviceConnected(String name, String address) {
				Toast.makeText(AppPlayBaseActivity.sTheActivity.getApplicationContext(),
						"Connected to " + name, Toast.LENGTH_SHORT).show();

				AppPlayNatives.nativeBluetoothOnConnected();
			}

			public void onDeviceDisconnected() {
				Toast.makeText(AppPlayBaseActivity.sTheActivity.getApplicationContext(), "Connection lost",
						Toast.LENGTH_SHORT).show();

				AppPlayNatives.nativeBluetoothOnDisConnected();
			}

			public void onDeviceConnectionFailed() {
				Log.i("Check", "Unable to connect");

				AppPlayNatives.nativeBluetoothOnConnectFailed();
			}
		});

		SetAutoConnectionListener(new AutoConnectionListener() {
			public void onNewConnection(String name, String address) {
				Log.i("Check", "New Connection - " + name + " - " + address);
			}

			public void onAutoConnectionStarted() {
				Log.i("Check", "Auto menu_connection started");
			}
		});	
		
	    mLeScanCallback = new BluetoothAdapter.LeScanCallback()
	    {  
	        @Override  
	        public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {  
	        	AppPlayBaseActivity.sTheActivity.runOnUiThread(new Runnable() 
	        	{  
	                @Override  
	                public void run() 
	                {
	                	String name = device.getName();
	                    String address = device.getAddress();
	                    
	                    if(!mLeDevices.contains(device))
	                    {
	                    	String strDevice = name + "_" + address;
	           				AppPlayBaseActivity.sTheActivity._BtOnDiscoveryNewDevice(strDevice);
	                                   	
	                        mLeDevices.add(device);
	                    }
	                }  
	            });  
	        }  
	    }; 
	    
	    mGattCallback = new BluetoothGattCallback() 
	    {  
	        @Override  //���������豸����ʧȥ����ʱ��ص��ú���  
	        public void onConnectionStateChange(BluetoothGatt gatt, int status,int newState) 
	        {  
	            if (newState == BluetoothProfile.STATE_CONNECTED)
	            { //
	            	mConnectionState = BluetoothState.STATE_CONNECTED;
	            	
	            	if (null != mBluetoothConnectionListener)
	            	{
	            		String name = gatt.getDevice().getName();
	            		String address = gatt.getDevice().getAddress();
	            		
	            		mBluetoothConnectionListener.onDeviceConnected(name, address);
	            		
	    				Log.i(TAG, "Connected to GATT server.");
	    				// Attempts to discover services after successful connection.
	    				Log.i(TAG, "Attempting to start service discovery:"
	    						+ mBluetoothGatt.discoverServices());
	            	}
	            }
	            else if (newState == BluetoothProfile.STATE_DISCONNECTING)
	            {
	            	mConnectionState = BluetoothState.STATE_NONE;
	            	
	            	if (null != mBluetoothConnectionListener)
	            	{
	            		mBluetoothConnectionListener.onDeviceConnectionFailed();
	            	} 
	            }
	            else if (newState == BluetoothProfile.STATE_DISCONNECTED) 
	            {
	            	mConnectionState = BluetoothState.STATE_NONE;
	            	
	            	if (null != mBluetoothConnectionListener)
	            	{
	            		mBluetoothConnectionListener.onDeviceDisconnected();
	            	}            	
	            }  
	        }  
	        @Override 
	        public void onServicesDiscovered(BluetoothGatt gatt, int status)
	        {  
	            if (status == BluetoothGatt.GATT_SUCCESS) 
	            {   
	            	//�ҵ�������  
	                //��������ԶԷ�����н�����Ѱ�ҵ�����Ҫ�ķ���  
	            }
	            else
	            {  
	                Log.w(TAG, "onServicesDiscovered received: " + status);  
	            }  
	        }  
	        
	        @Override  //����ȡ�豸ʱ��ص��ú���  
	        public void onCharacteristicRead(BluetoothGatt gatt,BluetoothGattCharacteristic characteristic, int status) 
	        {  
	            System.out.println("onCharacteristicRead");  
	            if (status == BluetoothGatt.GATT_SUCCESS) 
	            {  
	            	//��ȡ�������ݴ���characteristic���У�����ͨ��characteristic.getValue();����ȡ����Ȼ���ٽ��н���������  
	            	//int charaProp = characteristic.getProperties();if ((charaProp | BluetoothGattCharacteristic.PROPERTY_NOTIFY) > 0)��ʾ�ɷ���֪ͨ��  �жϸ�Characteristic����  
	            }  
	        }  
	  
	        @Override //�����豸Descriptor��д����ʱ����ص��ú���  
	        public void onDescriptorWrite(BluetoothGatt gatt,BluetoothGattDescriptor descriptor, int status) {  
	            System.out.println("onDescriptorWriteonDescriptorWrite = " + status + ", descriptor =" + descriptor.getUuid().toString());  
	        }  
	  
	        @Override //�豸����֪ͨʱ����õ��ýӿ�  
	        public void onCharacteristicChanged(BluetoothGatt gatt,BluetoothGattCharacteristic characteristic) {  
	            if (characteristic.getValue() != null) {  
	                  System.out.println(characteristic.getStringValue(0));  
	            }  
	            System.out.println("--------onCharacteristicChanged-----");  
	        }  
	  
	        @Override  
	        public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {  
	            System.out.println("rssi = " + rssi);  
	        }  
	        
		    @Override //����Characteristicд����ʱ��ص��ú���  
		    public void onCharacteristicWrite(BluetoothGatt gatt,BluetoothGattCharacteristic characteristic, int status) { 
		    	 byte[] bytes = characteristic.getValue();
		    	 int writeLength = bytes.length;	    	
		    	 
	             Log.w(TAG, "onCharacteristicWrite"+writeLength);  
		    	 
		    	 SendItemWirteBytes += writeLength;
		    	 
		    	 if (SendItemWirteBytes >= CurSendItemBytes)
		    	 {
			          SendItemWirteBytes -= CurSendItemBytes;
		    		 
			          Message msg = mBLESendHandler.obtainMessage(BLE_SEND_OK);
			          mBLESendHandler.sendMessage(msg);
		    	 }
		    }; 
	    };
    }
    
    private final Handler mBLESendHandler = new Handler()
    {
    	 public void handleMessage(Message msg) {
    		 switch(msg.what)
    		 {
    		 case BLE_SEND_OK:
    			 mIsCanSend = true;
    			 _SendAItem();
    			 break;
    		 }
    	 }
    };
    
    public boolean IsBluetoothAvailable() 
    {
    	if (null == mBluetoothAdapter)
    		return false;
    	
		if (!mContext.getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE))
		{
			return false;
		} 
		
		return true;
	}
    
    public boolean IsBluetoothEnabled() 
    {
        return mBluetoothAdapter.isEnabled();
    }
    
    public int GetConnectionState()
    {
    	return mConnectionState;
    }
    
    public Set<BluetoothDevice> GetBondedDevices()
    {
    	return mBluetoothAdapter.getBondedDevices();
    }
    
	public void Shutdown() 
	{
		mIsCanSend = true;
		SendItemWirteBytes = 0;
		mSendList.clear();
		
		if (mBluetoothGatt == null) 
		{
			return;
		}
		mBluetoothGatt.close();
		mBluetoothGatt = null;
	} 
    
    public void StartDiscovery() 
    {   	
    	mLeDevices.clear();
    	
        mHandler.postDelayed(new Runnable() 
        {  
            @Override  
            public void run() 
            {  
                mScanning = false;  
                mBluetoothAdapter.stopLeScan(mLeScanCallback);  
            }  
        }, SCAN_PERIOD); //10���ֹͣ����  
        
        mScanning = true;  
        
        mBluetoothAdapter.startLeScan(mLeScanCallback); //��ʼ����  
    }
    
    public void CancelDiscovery()
    {
        mScanning = false;  
        mBluetoothAdapter.stopLeScan(mLeScanCallback);//ֹͣ����  
    }
    
    public boolean Connect(String address)
    {  
    	mIsCanSend = true;
    	SendItemWirteBytes = 0;
		mSendList.clear();
		
        if (mBluetoothAdapter == null || address == null) 
        {  
            Log.w(TAG, "BluetoothAdapter not initialized or unspecified address.");  
            return false;  
        }  
        
        // Previously connected device. Try to reconnect. (��ǰ���ӵ��豸�� ������������)  
        if (mBluetoothDeviceAddress != null && 
        		address.equals(mBluetoothDeviceAddress) && 
        		mBluetoothGatt != null) 
        {  
            Log.d(TAG, "Trying to use an existing mBluetoothGatt for connection.");
            
            if (mBluetoothGatt.connect()) 
            {  
                mConnectionState = BluetoothState.STATE_CONNECTING;  
                return true;  
            }
            else
            {  
                return false;  
            }  
        }  
        
        final BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);  
        if (device == null) 
        {  
            Log.w(TAG, "Device not found.  Unable to connect.");  
            return false;  
        }  
        
        // �ú�������������ȥ��������  
        mBluetoothGatt = device.connectGatt(mContext, false, mGattCallback); 
        Log.d(TAG, "Trying to create a new connection.");  
        mBluetoothDeviceAddress = address;  
        mConnectionState = BluetoothState.STATE_CONNECTING;  
        
        return true;  
    }
    
	public void Disconnect() 
	{
		mIsCanSend = true;
		mSendList.clear();
		
		if (mBluetoothAdapter == null || mBluetoothGatt == null) 
		{
			Log.w(TAG, "BluetoothAdapter not initialized");
			return;
		}
		mBluetoothGatt.disconnect();
	}
	
	public void Send(byte[] data)
	{
        if(mConnectionState == BluetoothState.STATE_CONNECTED)
        {  
            mSendList.add(data);
            
            if (mIsCanSend)
            {             
            	_SendAItem();
            }
        }
	}
	
	private void _SendAItem()
	{
		CurSendItemBytes = 0;
		int count = mSendList.size();
		if (count > 0)
		{
			byte[] strData = mSendList.get(0);	
			_WriteDataToDevice(strData);
			
			mSendList.remove(0);			
	        mIsCanSend = false;	
		}
	}

	private void _WriteDataToDevice(byte[] data)
	{
        Log.i(TAG, "data:"+data + "length:"+data.length);
		
	    if (mBluetoothGatt == null) 
	        return;
	    
        // д���ݷ���
	    BluetoothGattService bluetoothGattServiceWrite = getSupportedGattServices(
	    		UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb"));//�����豸����Ҫʹ�õķ����UUID  
	    if (bluetoothGattServiceWrite == null)
	    {
	        Log.e(TAG, "service:"+mBluetoothGatt.getServices().size());  
	        Log.e(TAG, "service not found!");
	        return;  
	    }
	    
	    BluetoothGattCharacteristic mCharac = bluetoothGattServiceWrite  
	            .getCharacteristic(UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb"));
	    if (mCharac == null) 
	    {  
	        Log.e(TAG, "HEART RATE Copntrol Point charateristic not found!");  
	        return;  
	    }
	    
	    /*
	    // �����ݷ���
	    BluetoothGattService bluetoothGattServiceRead = getSupportedGattServices(
	    		UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb"));//�����豸����Ҫʹ�õķ����UUID  
	    if (bluetoothGattServiceRead == null)
	    {
	        Log.e(TAG, "service:"+mBluetoothGatt.getServices().size());  
	        Log.e(TAG, "service not found!");
	        return;  
	    }
	    
	    BluetoothGattCharacteristic nCharacteristicRead =bluetoothGattServiceRead
	    		.getCharacteristic(UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb"));
	    setCharacteristicNotification(nCharacteristicRead, true); 
	    */
	    
	    mCharac.setValue(data); //������Ҫ���͵����� 	  
   
	    if (null!=mBluetoothGatt)
	    	mBluetoothGatt.writeCharacteristic(mCharac);
	}  
	
	public void setCharacteristicNotification(
			BluetoothGattCharacteristic characteristic, boolean enabled) 
	{
		if (mBluetoothAdapter == null || mBluetoothGatt == null) 
		{
			Log.w(TAG, "BluetoothAdapter not initialized");
			return;
		}
		
		mBluetoothGatt.setCharacteristicNotification(characteristic, enabled);		
	}
     
    // listener    
    public void SetBluetoothStateListener (BluetoothListener.BluetoothStateListener listener) 
    {
        mBluetoothStateListener = listener;
    }
    
    public void setOnDataReceivedListener (BluetoothListener.OnDataReceivedListener listener) 
    {
        mDataReceivedListener = listener;
    }
    
    public void SetBluetoothConnectionListener (BluetoothListener.BluetoothConnectionListener listener)
    {
        mBluetoothConnectionListener = listener;
    }
    
    public void SetAutoConnectionListener(BluetoothListener.AutoConnectionListener listener)
    {
        mAutoConnectionListener = listener;
    }
    
    // binder
	public class LocalBinder extends Binder 
	{
		BluetoothLE getService() {
			return BluetoothLE.this;
		}
	}

	@Override
	public IBinder onBind(Intent intent) 
	{
		return mBinder;
	}
	
	private final IBinder mBinder = new LocalBinder();

    public List<BluetoothGattService> getSupportedGattServices() 
    {
        if (mBluetoothGatt == null) return null;

        return mBluetoothGatt.getServices();
    }
    public BluetoothGattService getSupportedGattServices(UUID uuid) 
    {
    	BluetoothGattService bluetoothGattService;
    	
    	if (mBluetoothGatt == null) 
    		return null;
    	
    	bluetoothGattService=mBluetoothGatt.getService(uuid);    	
        return bluetoothGattService;
    }
	
}
