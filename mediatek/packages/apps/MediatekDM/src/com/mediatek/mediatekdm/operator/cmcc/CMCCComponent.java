package com.mediatek.mediatekdm.operator.cmcc;

import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import com.mediatek.common.dm.DmAgent;
import com.mediatek.mediatekdm.DmApplication;
import com.mediatek.mediatekdm.DmConfig;
import com.mediatek.mediatekdm.DmConst.TAG;
import com.mediatek.mediatekdm.DmFeatureSwitch;
import com.mediatek.mediatekdm.DmOperation;
import com.mediatek.mediatekdm.DmService;
import com.mediatek.mediatekdm.IDmComponent;
import com.mediatek.mediatekdm.PlatformManager;
import com.mediatek.mediatekdm.SessionHandler;
import com.mediatek.mediatekdm.mdm.MdmTree;
import com.mediatek.mediatekdm.mdm.SessionInitiator;
import com.mediatek.mediatekdm.mdm.SessionStateObserver.SessionState;
import com.mediatek.mediatekdm.mdm.SessionStateObserver.SessionType;
import com.mediatek.telephony.SimInfoManager.SimInfoRecord;

import java.util.List;

public class CMCCComponent implements IDmComponent {
    static final String NAME = "CMCC";

    private DmService mService = null;

    @Override
    public boolean acceptOperation(SessionInitiator initiator, DmOperation operation) {
        return false;
    }

    @Override
    public void attach(DmService service) {
        mService = service;
    }

    @Override
    public void detach(DmService service) {
        mService = null;
    }

    @Override
    public DispatchResult dispatchBroadcast(Context context, Intent intent) {
        return DispatchResult.IGNORE;
    }

    @Override
    public DispatchResult dispatchCommand(Intent intent) {
        return DispatchResult.IGNORE;
    }

    @Override
    public void dispatchMmiProgressUpdate(DmOperation operation, int current, int total) {
    }

    @Override
    public DispatchResult dispatchOperationAction(OperationAction action, DmOperation operation) {
        return DispatchResult.IGNORE;
    }

    @Override
    public SessionHandler dispatchSessionStateChange(
            SessionType type,
            SessionState state,
            int lastError,
            SessionInitiator initiator,
            DmOperation operation) {
        return null;
    }

    @Override
    public boolean forceSilentMode() {
        return false;
    }

    @Override
    public IBinder getBinder(Intent intent) {
        return null;
    }

    @Override
    public String getDlPackageFilename() {
        return null;
    }

    @Override
    public String getDlResumeFilename() {
        return null;
    }

    @Override
    public String getName() {
        return NAME;
    }

    @Override
    public void kickoff(Context context) {
    }

    @Override
    public void configureDmTree(MdmTree tree) {
        String mccmnc = null;
        Context context = DmApplication.getInstance();
        if (DmConfig.getInstance().useSmsReg()) {
            int simSlot = getRegisteredSimId(context);
            if (simSlot != -1) {
                mccmnc = PlatformManager.getInstance().getSimOperator(simSlot);
                Log.w(TAG.CONTROLLER, "reg sim MccMnc: " + mccmnc);
                IoNodeHandlerRegister dr = new IoNodeHandlerRegister(context, mccmnc);
                dr.registerSettingsNodeIoHandler();
            }
        }

    }

    /**
     * getRegisteredSimId
     * 
     * @param context
     * @return: -1 -- no sim registered 0 -- Device support one sim and is registered other -- registered sim ID.
     */
    public static int getRegisteredSimId(Context context) {
        String registerIMSI = null;

        try {
            DmAgent agent = PlatformManager.getInstance().getDmAgent();
            if (agent == null) {
                Log.e(TAG.COMMON, "get dm_agent_binder failed.");
                return -1;
            }
            byte[] imsiData = agent.readImsi();
            if (imsiData == null) {
                Log.e(TAG.COMMON, "get imsi failed.");
                return -1;
            }
            registerIMSI = new String(imsiData);
        } catch (RemoteException e) {
            Log.e(TAG.COMMON, "get registered IMSI failed", e);
        }

        if (registerIMSI == null) {
            Log.e(TAG.COMMON, "get registered IMSI failed");
            return -1;
        }

        Log.i(TAG.COMMON, "registered imsi=" + registerIMSI);

        List<SimInfoRecord> simList = PlatformManager.getInstance().getInsertedSimInfoList(context);
        for (SimInfoRecord sim : simList) {
            String imsi = PlatformManager.getInstance().getSubscriberId(sim.mSimSlotId);
            Log.d(TAG.COMMON, "Sim" + sim.mSimSlotId + ":imsi=" + imsi);
            if (imsi != null && imsi.equals(registerIMSI)) {
                Log.d(TAG.COMMON, "registered SIM card is SIM" + sim.mSimSlotId);
                return sim.mSimSlotId;
            }
        }

        Log.d(TAG.COMMON, "getRegisteredSimId error!");
        return -1;
    }

    @Override
    public DispatchResult validateWapPushMessage(Intent intent) {
        if (!DmConfig.getInstance().useSmsReg()) {
            return DispatchResult.ACCEPT;
        }

        // Check whether the NIA came from the registered SIM card
        int registeredSimId = getRegisteredSimId(mService);
        if (DmFeatureSwitch.MTK_GEMINI_SUPPORT) {
            int receivedSimId = intent.getIntExtra("simId", -1);
            if (receivedSimId != registeredSimId) {
                Log.w(TAG.SERVICE, "NIA is not from the registered card. Do nothing.");
                Log.d(TAG.SERVICE, "receivedSimId = " + receivedSimId + ", registeredSimId = " + registeredSimId);
                return DispatchResult.ABORT;
            } else {
                return DispatchResult.ACCEPT;
            }
        } else {
            return DispatchResult.ACCEPT;
        }
    }

    @Override
    public boolean checkPrerequisites() {
        if (!DmConfig.getInstance().useSmsReg()) {
            return true;
        }

        return !(getRegisteredSimId(mService) == -1);
    }
}
