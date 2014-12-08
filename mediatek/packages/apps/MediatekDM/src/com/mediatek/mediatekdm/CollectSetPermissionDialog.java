package com.mediatek.mediatekdm;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.DialogInterface.OnClickListener;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.CheckBox;
import android.widget.TextView;

import com.android.internal.app.AlertActivity;
import com.android.internal.app.AlertController;
import com.mediatek.xlog.Xlog;

public class CollectSetPermissionDialog extends Activity {
    
    private static final int DIALOG_ID_COLLECT_SET_PERMISSION= 1;
    
    private NotificationManager mNotificationManager = null;
    Notification mNotification = null;
    CheckBox mCheckBox = null;
    TextView msgTxt = null;
    //(NotificationManager) service.getSystemService(Context.NOTIFICATION_SERVICE);
    //private CheckBox mCheckBox;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        //createDialog();
        
        mNotificationManager = (NotificationManager) this.getSystemService(Context.NOTIFICATION_SERVICE);
        showDialog(DIALOG_ID_COLLECT_SET_PERMISSION);
        Xlog.i(DmConst.TAG.COLLECT_SET_DIALOG, "CollectSetPermissionDialog createDialog" );
        //showDialog(id);
        
    }
    @Override
    protected Dialog onCreateDialog(int id) {
        Log.d(DmConst.TAG.COLLECT_SET_DIALOG, "onCreateDialog with id " + id);
        Dialog dialog = null;
        switch (id) {
            case DIALOG_ID_COLLECT_SET_PERMISSION:
                dialog = collectSetPermissionDialog();
                Log.d(DmConst.TAG.COLLECT_SET_DIALOG, "collectSetPermissionDialog" + id);
                sendNotification();
                break;
        }
        return dialog;
    }
    
    private void sendNotification() {
        mNotification = new Notification();  
        mNotification.icon = R.drawable.perm_group_turn_on_data_connection;   
        mNotification.tickerText = getResources().getString(R.string.collect_set_permission_notification_tickerText);
        mNotification.flags = Notification.FLAG_NO_CLEAR;
        mNotification.audioStreamType= android.media.AudioManager.ADJUST_LOWER;  
        Intent intent = getIntent();
        intent.setClass(this, CollectSetPermissionDialog.class); 
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);  
        mNotification.setLatestEventInfo(this, 
                getResources().getString(R.string.collect_set_permission_notification_title),
                getResources().getString(R.string.collect_set_permission_notification_msg), 
                pendingIntent);
        mNotificationManager.notify(DmConst.NotificationInteractionType.TYPE_COLLECT_SET_PERM_NOTIFICATION, mNotification);
    }
    private Dialog collectSetPermissionDialog() {
        Log.d(DmConst.TAG.COLLECT_SET_DIALOG, "collectSetPermissionDialog()");
        Dialog dialog = null;
        LayoutInflater inflater = LayoutInflater.from(this);
        View layout = inflater.inflate(R.layout.notify_dialog_customview, null);
        mCheckBox = (CheckBox)layout.findViewById(R.id.checkbox);
        msgTxt = (TextView)layout.findViewById(R.id.message);
        msgTxt.setText(R.string.collect_set_permission_dlg_mgs);
        
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setCancelable(false);
        builder.setTitle(R.string.collect_set_permission_dlg_title);
        builder.setView(layout);  
        builder.setPositiveButton(R.string.alert_dlg_ok_button, new OnClickListener() {  
           boolean isNeedNotify = true;
           boolean isNeedAgree = true;
           @Override 
           public void onClick(DialogInterface dialog, int which) {
               Xlog.i(DmConst.TAG.COLLECT_SET_DIALOG, "COLLECT SET yes" );

               if (mCheckBox.isChecked()) {
                   isNeedNotify = false;
               } else {
                   isNeedNotify = true;
               }
               mNotificationManager.cancel(DmConst.NotificationInteractionType.TYPE_COLLECT_SET_PERM_NOTIFICATION);
               dialog.dismiss();
               responsePermissionDialog(isNeedAgree,isNeedNotify);
               CollectSetPermissionDialog.this.finish();
           }
        });   
        builder.setNegativeButton(R.string.alert_dlg_cancel_button, new OnClickListener() {  
            boolean isNeedNotify = true;
            boolean isNeedAgree = true;
            @Override 
            public void onClick(DialogInterface dialog, int which) {
                Xlog.i(DmConst.TAG.COLLECT_SET_DIALOG, "COLLECT SET no" );

                if (mCheckBox.isChecked()) {
                    isNeedNotify = false;
                } else {
                    isNeedNotify = true;
                }
                dialog.dismiss();
                AlertDialog.Builder confirmBuilder = new AlertDialog.Builder(CollectSetPermissionDialog.this);
                confirmBuilder.setCancelable(false);
                confirmBuilder.setTitle(R.string.confirm_dlg_title);
                confirmBuilder.setMessage(R.string.confirm_dlg__msg);  

                confirmBuilder.setPositiveButton(R.string.alert_dlg_ok_button, new OnClickListener() {  
                    @Override 
                    public void onClick(DialogInterface dialog, int which) {  
                        Xlog.i(DmConst.TAG.COLLECT_SET_DIALOG, "notify yes" );
                        mNotificationManager.cancel(DmConst.NotificationInteractionType.TYPE_COLLECT_SET_PERM_NOTIFICATION);
                        dialog.dismiss();
                        isNeedAgree = false;
                        responsePermissionDialog(isNeedAgree,isNeedNotify);
                        CollectSetPermissionDialog.this.finish();
                    } 
                });
                confirmBuilder.setNegativeButton(R.string.alert_dlg_cancel_button, new OnClickListener() {  
                    @Override 
                    public void onClick(DialogInterface dialog, int which) {  
                        Xlog.i(DmConst.TAG.COLLECT_SET_DIALOG, "notify no" );
                        mNotificationManager.cancel(DmConst.NotificationInteractionType.TYPE_COLLECT_SET_PERM_NOTIFICATION);
                        dialog.dismiss();
                        Intent intent = getIntent();
                        CollectSetPermissionDialog.this.finish();
                        intent.setAction(DmConst.IntentAction.DM_COLLECT_SET_DIALOG_START);
                        startActivity(intent);
                        
                    } 
                });
                confirmBuilder.show();
                Xlog.i(DmConst.TAG.COLLECT_SET_DIALOG, "confirmBuilder.show();" );
                
            }
        });
        builder.setCancelable(false);
        dialog =builder.create();
        return dialog;
    }
    public void responsePermissionDialog(boolean isNeedAgree,boolean isNeedNotify) {
        Intent intent = getIntent();
        intent.putExtra(DmConst.ExtraKey.IS_NEED_AGREE, isNeedAgree);
        intent.putExtra(DmConst.ExtraKey.IS_NEED_NOTIFY, isNeedNotify);
        intent.setAction(DmConst.IntentAction.DM_COLLECT_SET_DIALOG_END);
        intent.setClass(this, DmReceiver.class);
        sendBroadcast(intent);
    }
}

