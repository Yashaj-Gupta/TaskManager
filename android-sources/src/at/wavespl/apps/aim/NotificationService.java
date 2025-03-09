package acc.u.merrite.aim;//later make it as it planned

import android.content.Context;
import android.content.Intent;
import org.qtproject.qt5.android.bindings.QtService;

import android.util.Log;

import android.app.IntentService;

//import at.wavespl.apps.aim.NotificationClient;

public class NotificationService extends QtService { //QtService https://www.kdab.com/qt-android-create-android-service-using-qt/

    public static void startMyService(Context ctx) {
           ctx.startService(new Intent(ctx, NotificationService.class));
       }

}
