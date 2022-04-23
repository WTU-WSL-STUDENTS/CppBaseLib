package com.example.myapplication.slice;

import ohos.aafwk.ability.AbilitySlice;
import ohos.aafwk.content.Intent;
import ohos.agp.components.PositionLayout;
import ohos.agp.components.Text;
import ohos.agp.text.Font;
import ohos.agp.utils.Color;
import ohos.agp.colors.RgbColor;
import ohos.agp.components.element.ShapeElement;
import ohos.agp.components.ComponentContainer.LayoutConfig;
import ohos.hiviewdfx.HiLog;
import ohos.hiviewdfx.HiLogLabel;

public class MainAbilitySlice extends AbilitySlice {
    // Load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("entry");
    }

    private final PositionLayout myLayout = new PositionLayout(this);
    private final HiLogLabel label = new HiLogLabel(0, 0, "JNI debug from java");
    @Override
    public void onStart(Intent intent) {
        super.onStart(intent);
        LayoutConfig config = new LayoutConfig(LayoutConfig.MATCH_PARENT, LayoutConfig.MATCH_PARENT);
        myLayout.setLayoutConfig(config);
        ShapeElement element = new ShapeElement();
        element.setShape(ShapeElement.RECTANGLE);
        element.setRgbColor(new RgbColor(255, 255, 255));
        myLayout.setBackground(element);
        Text text = new Text(this);
        text.setText(stringFromJNI());
        text.setTextColor(Color.BLACK);
        int nums[] = {1, 2, 3, 4, 5};
        HiLog.fatal(label, JNITypeTest(true, 3.14f, 0xff, nums));
        HiLog.fatal(label, stringFromJNI());
        String str = new String();
        for(int i = 0; i < 5; i++)
        {
            str += nums[i];
            str += ",";
        }
        HiLog.fatal(label, str);
        myLayout.addComponent(text);
        super.setUIContent(myLayout);
    }

    @Override
    public void onActive() {
        super.onActive();
    }

    @Override
    public void onForeground(Intent intent) {
        super.onForeground(intent);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native String JNITypeTest(boolean js, float f1, long l1, int[] nums);

}
