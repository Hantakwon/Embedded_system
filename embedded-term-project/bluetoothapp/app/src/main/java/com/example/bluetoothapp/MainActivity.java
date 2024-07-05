package com.example.bluetoothapp;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TableLayout;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {
    private BluetoothAdapter bluetoothAdapter;
    private BluetoothSocket mmSocket;
    private InputStream mmInStream;
    private OutputStream mmOutStream;
    private ArrayAdapter<String> pairedDeviceAdapter;

    // 권한 요청 코드
    private static final int REQUEST_ENABLE_BT = 1;
    private static final int REQUEST_PERMISSION_BT_CONNECT = 2;
    private static final int REQUEST_PERMISSION_BT_SCAN = 3;

    // UI 요소
    private TextView receivedDataTextView, doorlockStateView, temperatureView, humidityView;
    private EditText sendDataEditText;

    // BUTTON 요소
    private Button openBtn, closeBtn;

    private final BroadcastReceiver bluetoothReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            if (action != null && (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED))) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR);
                if (state == BluetoothAdapter.STATE_ON || state == BluetoothAdapter.STATE_OFF) {
                    getPairedDevices(pairedDeviceAdapter);
                }
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // BluetoothAdapter 인스턴스 가져오기
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // UI 요소 초기화
        receivedDataTextView = findViewById(R.id.receivedDataTextView);
        doorlockStateView = findViewById(R.id.doorlookStateView);
        temperatureView = findViewById(R.id.temperatureView);
        humidityView = findViewById(R.id.humidityView);
        TextView receivedData = (TextView) findViewById(R.id.receivedDataTextView);
        TableLayout table = (TableLayout) findViewById(R.id.tableLayout);
        openBtn = findViewById(R.id.openBtn);
        closeBtn = findViewById(R.id.closeBtn);

        sendDataEditText = findViewById(R.id.sendDataEditText);
        Button btnSendData = findViewById(R.id.btnSendData);
        btnSendData.setOnClickListener(view -> {
            String message = (sendDataEditText.getText().toString()+"\n");
            if (!message.isEmpty()) {
                new Thread(() -> sendMessage(message)).start(); // 비동기로 데이터 전송
                sendDataEditText.setText(""); // EditText 내용 삭제
            }
        });

        // Bluetooth ON 버튼 설정
        Button btnBluetoothOn = findViewById(R.id.btnBluetoothOn);
        btnBluetoothOn.setOnClickListener(view -> {
            // Bluetooth가 지원되지 않는 기기인 경우
            if (bluetoothAdapter == null) {
                Toast.makeText(getApplicationContext(), "Bluetooth를 지원하지 않는 기기입니다.", Toast.LENGTH_SHORT).show();
                return;
            }
            // Bluetooth가 켜져 있지 않은 경우
            if (!bluetoothAdapter.isEnabled()) {
                receivedData.setVisibility(View.VISIBLE);
                table.setVisibility(View.VISIBLE);
                openBtn.setVisibility(View.VISIBLE);
                closeBtn.setVisibility(View.VISIBLE);
                sendDataEditText.setVisibility(View.VISIBLE);
                btnSendData.setVisibility(View.VISIBLE);
                // Bluetooth를 켜도록 사용자에게 요청하는 인텐트 생성
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        });

        // Bluetooth OFF 버튼 설정
        Button btnBluetoothOff = findViewById(R.id.btnBluetoothOff);
        btnBluetoothOff.setOnClickListener(view -> {
            // Bluetooth가 켜져 있는 경우
            if (bluetoothAdapter.isEnabled()) {
                // 권한 확인
                if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, REQUEST_PERMISSION_BT_CONNECT);
                } else {
                    // Bluetooth 비활성화
                    bluetoothAdapter.disable();
                    receivedData.setVisibility(View.INVISIBLE);
                    table.setVisibility(View.INVISIBLE);
                    openBtn.setVisibility(View.INVISIBLE);
                    closeBtn.setVisibility(View.INVISIBLE);
                    sendDataEditText.setVisibility(View.INVISIBLE);
                    btnSendData.setVisibility(View.INVISIBLE);
                    Toast.makeText(getApplicationContext(), "Bluetooth가 꺼졌습니다.", Toast.LENGTH_SHORT).show();
                }
            }
        });

        // 페어링된 기기 목록을 보여줄 ListView 설정
        ListView pairedDeviceListView = findViewById(R.id.pairedDeviceList);
        pairedDeviceAdapter = new ArrayAdapter<>(this, android.R.layout.simple_list_item_1);
        pairedDeviceListView.setAdapter(pairedDeviceAdapter);

        // 권한 요청 및 페어링된 기기 목록 가져오기
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, REQUEST_PERMISSION_BT_CONNECT);
        } else {
            getPairedDevices(pairedDeviceAdapter);
        }

        // ListView 항목 클릭 리스너 설정
        pairedDeviceListView.setOnItemClickListener((parent, view, position, id) -> {
            // 클릭한 항목에서 기기 정보 추출
            String item = (String) parent.getItemAtPosition(position);
            String address = item.split("\n")[1];  // 기기의 MAC 주소 추출
            BluetoothDevice device = bluetoothAdapter.getRemoteDevice(address);

            // 연결 시도 중 메시지 표시
            Toast.makeText(getApplicationContext(), "연결 시도 중입니다.", Toast.LENGTH_SHORT).show();

            // 기기와 연결 시도
            ConnectThread connectThread = new ConnectThread(device);
            connectThread.start();
        });

        // Bluetooth 상태 변경 수신기 등록
        IntentFilter filter = new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED);
        registerReceiver(bluetoothReceiver, filter);

        // Open 버튼 동작
        openBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String message = "DOOR=OPEN\n";
                if (!message.isEmpty()) {
                    new Thread(() -> sendMessage(message)).start(); // 비동기로 데이터 전송
                    sendDataEditText.setText(""); // EditText 내용 삭제
                }
            }
        });

        // Close 버튼 동작
        closeBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String message = "DOOR=LOCK\n";
                if (!message.isEmpty()) {
                    new Thread(() -> sendMessage(message)).start(); // 비동기로 데이터 전송
                    sendDataEditText.setText(""); // EditText 내용 삭제
                }
            }
        });
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        // BroadcastReceiver 해제
        receivedDataTextView.setText("ReciviedData");
        unregisterReceiver(bluetoothReceiver);
    }

    // 페어링된 기기 목록 가져오기
    private void getPairedDevices(ArrayAdapter<String> pairedDeviceAdapter) {
        try {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, REQUEST_PERMISSION_BT_CONNECT);
                return;
            }
            Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
            pairedDeviceAdapter.clear();  // 기존 목록 지우기
            // 페어링된 기기가 있는 경우 목록에 추가
            if (pairedDevices.size() > 0) {
                for (BluetoothDevice device : pairedDevices) {
                    pairedDeviceAdapter.add(device.getName() + "\n" + device.getAddress());
                }
            }
        } catch (SecurityException e) {
            e.printStackTrace();
            Toast.makeText(this, "Bluetooth 권한이 필요합니다.", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == REQUEST_PERMISSION_BT_CONNECT) {
            if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                getPairedDevices(pairedDeviceAdapter);
            } else {
                Toast.makeText(this, "Bluetooth 권한이 필요합니다.", Toast.LENGTH_SHORT).show();
            }
        }
    }

    private class ConnectThread extends Thread {
        private final BluetoothDevice device;
        private BluetoothSocket socket;

        public ConnectThread(BluetoothDevice device) {
            BluetoothSocket tmp = null;
            this.device = device;

            try {
                if (ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, REQUEST_PERMISSION_BT_CONNECT);
                    return;
                }
                // UUID는 기기마다 다를 수 있음. 적절한 UUID를 사용해야 함
                tmp = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
            } catch (IOException e) {
                e.printStackTrace();
            }
            socket = tmp;
        }

        public void run() {
            // 권한 체크
            try {
                if (ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.BLUETOOTH_SCAN}, REQUEST_PERMISSION_BT_SCAN);
                    return;
                }

                // Bluetooth 검색 중지
                bluetoothAdapter.cancelDiscovery();

                try {
                    if (ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                        ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, REQUEST_PERMISSION_BT_CONNECT);
                        return;
                    }
                    // Bluetooth 소켓 연결 시도
                    socket.connect();
                    runOnUiThread(() -> Toast.makeText(getApplicationContext(), "기기와 연결되었습니다.", Toast.LENGTH_SHORT).show());
                    manageConnectedSocket(socket);  // 연결된 소켓 관리 시작
                } catch (IOException connectException) {
                    try {
                        socket.close();
                    } catch (IOException closeException) {
                        closeException.printStackTrace();
                    }
                }
            } catch (SecurityException e) {
                e.printStackTrace();
                runOnUiThread(() -> Toast.makeText(getApplicationContext(), "Bluetooth 권한이 필요합니다.", Toast.LENGTH_SHORT).show());
            }
        }
    }

    // 연결된 소켓을 관리하는 메서드
    private void manageConnectedSocket(BluetoothSocket socket) {
        mmSocket = socket;
        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        try {
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) {
            e.printStackTrace();
        }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;

        // 데이터를 읽는 스레드
        new Thread(() -> {
            byte[] buffer = new byte[1024];
            int bytes;
            Handler handler = new Handler(Looper.getMainLooper());
            StringBuilder readBuffer = new StringBuilder();

            while (true) {
                try {
                    // InputStream으로부터 데이터를 읽음
                    bytes = mmInStream.read(buffer);
                    String readMessage = new String(buffer, 0, bytes);
                    readBuffer.append(readMessage);

                    int index;
                    while ((index = readBuffer.indexOf("\n")) != -1) {
                        // 한 줄 단위로 읽어오기
                        String completeMessage = readBuffer.substring(0, index + 1);
                        readBuffer.delete(0, index + 1);

                        String[] parts = completeMessage.split("=");

                        if (parts.length >= 2) {
                            handler.post(() -> {
                                try {
                                    if (parts[0].equals("door") && doorlockStateView != null) {
                                        doorlockStateView.setText(parts[1]);
                                    } else if (parts[0].equals("temp") && temperatureView != null) {
                                        temperatureView.setText(parts[1]);
                                    } else if (parts[0].equals("humidity") && humidityView != null) {
                                        humidityView.setText(parts[1]);
                                    }
                                } catch (Exception e) {
                                    Log.e("BluetoothApp", "Error updating UI", e);
                                }
                            });
                        }
                        else {
                            if (receivedDataTextView != null) {
                                handler.post(() -> {
                                    receivedDataTextView.setText(completeMessage.trim());
                                });
                            }
                        }
                    }
                } catch (IOException e) {
                    break;
                }
            }
        }).start();
    }

    // 메시지를 전송하는 메서드
    private void sendMessage(String message) {
        byte[] bytes = message.getBytes();
        try {
            if (mmOutStream != null) {
                mmOutStream.write(bytes);
                mmOutStream.flush(); // 즉시 전송
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
