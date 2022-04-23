package com.example.myapplication;

import com.example.myapplication.slice.MainAbilitySlice;
import com.example.myapplication.hardware.mobilephone.camera.*;
import ohos.aafwk.ability.Ability;
import ohos.aafwk.content.Intent;
import ohos.agp.components.ComponentContainer;
import ohos.agp.graphics.SurfaceOps;
import ohos.app.AbilityContext;
import ohos.hiviewdfx.HiLog;
import ohos.hiviewdfx.HiLogLabel;
import ohos.media.camera.CameraKit;
import ohos.media.camera.device.CameraAbility;
import ohos.media.camera.device.CameraInfo;
import ohos.media.image.common.ImageFormat;
import ohos.media.image.common.Size;
import java.util.*;

public class MainAbility extends Ability {
    CameraController camera;
    HiLogLabel label = new HiLogLabel(0, 0, "MainAbility");
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        super.setMainRoute(MainAbilitySlice.class.getName());

        camera = new CameraController(this);
        // HiLog.fatal(label, GetCameraDeviceDetails(camera));
        CameraObject cameraObject = new CameraObject(camera, "0");
//        CameraFrameController cameraFrame = new CameraFrameController(this,
//            (ComponentContainer)findComponentById(ResourceTable.String_entry_MainAbility));
//        cameraObject.LockDevice(camera, cameraFrame);
//        HiLog.fatal(label, cameraObject.Configure() ? "Camera configure sucess" : "Camera configure failed");
        // cameraObject.ReleaseDevice();
    }

    /* 获取所有未占用相机的详细描述信息 */
    public String GetCameraDeviceDetails(CameraController camera){
        String[] cameraList = camera.TryGetCameraIdList();
        StringBuilder sb = new StringBuilder();
        sb.append(String.format("Camera devices count : %d\n", cameraList.length));
        CameraObject[] cameraObjects = new CameraObject[cameraList.length];
        for (int i = 0; i < cameraList.length; i++){
            cameraObjects[i] = new CameraObject(camera, cameraList[i]);
            sb.append(String.format("  Camera-%s info\n", cameraList[i]));
            try{
                sb.append(GetCameraInfoDescription(cameraObjects[i].info));
            }catch (Exception e){
                HiLog.fatal(label, "catched exception in GetCameraInfoDescription : " + e.getMessage());
            }
            sb.append(String.format("  Camera-%s ability\n", cameraList[i]));
            try {
                sb.append(GetCameraAbilityDescription(cameraObjects[i].ability));
            }catch (Exception e){
                HiLog.fatal(label, "catched exception in GetCameraAbilityDescription : " + e.getMessage());
            }
        }
        return sb.toString();
    }
    private String GetCameraAbilityDescription(CameraAbility ability){
        StringBuilder sb = new StringBuilder();
        /* 各种图像支持的分辨率 */
        for (int i = ImageFormat.UNKNOWN; i <= ImageFormat.H264; i++) {
            final int nType = i;
            ability.getSupportedSizes(i).forEach((Size s)->{
                sb.append(String.format("    Type-%d , supported size(%d, %d)\n", nType, s.width, s.height));
            });
        }
        /* 自动曝光模式 */
        int autoexposure[] = ability.getSupportedAeMode();
        for (int i = 0; i < autoexposure.length; i++){
            sb.append(String.format("    Auto exposure support : %d\n", autoexposure[i]));
        }
        /* 自动对焦模式 */
        int autofocus[] = ability.getSupportedAfMode();
        for (int i = 0; i < autofocus.length; i++){
            sb.append(String.format("    Auto focus support : %d\n", autofocus[i]));
        }
        /* 人脸检测类型范围 */
        int faceDetection[] = ability.getSupportedFaceDetection();
        for (int i = 0; i < faceDetection.length; i++){
            sb.append(String.format("    Face detection support : %d\n", faceDetection[i]));
        }
        /* 闪光灯取值范围 */
        int flashLight[] = ability.getSupportedFlashMode();
        for (int i = 0; i < flashLight.length; i++){
            sb.append(String.format("    Flash light support : %d\n", flashLight[i]));
        }
        /* 变焦范围 */
        float zoomRange[] = ability.getSupportedZoom();
        for(int i = 0; i < zoomRange.length; i++){
            sb.append(String.format("    Zoom support : %f\n", zoomRange[i]));
        }
        return sb.toString();
    }
    private String GetCameraInfoDescription(CameraInfo info){
        StringBuilder sb = new StringBuilder();
        /* 相机朝向信息 */
        int nFacingDirection = info.getFacingType();
        sb.append(String.format("    Facing direction : %d\n", nFacingDirection));/* 0 前置相机, 1 后置相机*/
        /* 逻辑相机ID */
        sb.append(String.format("    Logical id : %s\n", info.getLogicalId()));
        /* 物理相机连接方式 */
        info.getPhysicalIdList().forEach(id->{
            sb.append(String.format("    Physical id : %s, link type : %d\n", id, info.getDeviceLinkType(id)));
        });/* 0 内置相机, 1 USB-相机*/
        return  sb.toString();
    }

}
