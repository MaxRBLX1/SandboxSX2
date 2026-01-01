package com.maxrblx1.sandboxsx2

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.maxrblx1.sandboxsx2.ui.theme.SandboxSX2Theme
import kotlinx.coroutines.delay
import java.io.File

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()

        // Base directory for app files
        val baseDir = getExternalFilesDir(null) ?: filesDir

        // Ensure ps2_bios folder exists
        val biosDir = File(baseDir, "ps2_bios")
        if (!biosDir.exists()) {
            biosDir.mkdirs()
        }

        setContent {
            SandboxSX2Theme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    EmulatorScreen(modifier = Modifier.padding(innerPadding), biosDir = biosDir)
                }
            }
        }
    }
}

@Composable
fun EmulatorScreen(modifier: Modifier = Modifier, biosDir: File) {
    val emulator = NativeEmulator.instance

    var isRunning by remember { mutableStateOf(false) }
    var tickCount by remember { mutableLongStateOf(0L) }
    var pc by remember { mutableIntStateOf(0) }
    var debugState by remember { mutableStateOf("") }
    var biosWarning by remember { mutableStateOf("") }
    var biosLoaded by remember { mutableStateOf(false) }

    // BIOS loader
    LaunchedEffect(Unit) {
        val biosFiles = biosDir.listFiles()
        if (biosFiles.isNullOrEmpty()) {
            biosWarning = "No BIOS found in /ps2_bios/"
        } else {
            val validFile = biosFiles.firstOrNull { it.isFile && it.name.lowercase().endsWith(".bin") }
            if (validFile != null) {
                val bytes = validFile.readBytes()
                val ok = emulator.nativeLoadBiosPart("bin", bytes)
                biosLoaded = ok
                biosWarning = if (ok) "" else "Failed to load BIOS: ${validFile.name}"
            } else {
                biosWarning = "No valid BIOS file (.bin) found"
            }
        }
    }

    // VM loop
    LaunchedEffect(isRunning) {
        if (isRunning && biosLoaded) {
            while (isRunning) {
                emulator.nativeTick()
                tickCount = emulator.nativeGetTickCount()
                pc = emulator.nativeGetPC()
                debugState = emulator.nativeGetDebugState()
                delay(16) // ~60 ticks per second
            }
        }
    }

    val scrollState = rememberScrollState()

    Column(
        modifier = modifier
            .padding(16.dp)
            .fillMaxSize()
            .verticalScroll(scrollState)
    ) {
        Text("SandboxSX2 v0.5", style = MaterialTheme.typography.headlineMedium, color = Color(0xFF6200EE))
        Spacer(Modifier.height(16.dp))

        // BIOS status
        if (biosWarning.isNotEmpty()) {
            Text(biosWarning, color = MaterialTheme.colorScheme.error)
        } else if (biosLoaded) {
            Text("BIOS loaded successfully", color = MaterialTheme.colorScheme.primary)
        }

        Spacer(Modifier.height(16.dp))

        // Controls
        Row(horizontalArrangement = Arrangement.spacedBy(8.dp)) {
            Button(
                onClick = { if (biosLoaded) isRunning = !isRunning },
                modifier = Modifier.weight(1f)
            ) {
                Text(
                    when {
                        !biosLoaded -> "BIOS Required"
                        isRunning -> "Pause VM Loop"
                        else -> "Start VM Loop"
                    }
                )
            }
        }

        Spacer(Modifier.height(16.dp))

        // Debug overlay
        Card(
            modifier = Modifier.fillMaxWidth(),
            colors = CardDefaults.cardColors(containerColor = Color(0xFFF3E5F5))
        ) {
            Column(modifier = Modifier.padding(12.dp)) {
                Text("Debug Overlay", style = MaterialTheme.typography.titleMedium, color = Color(0xFF6A1B9A))
                Spacer(Modifier.height(8.dp))
                Text("PC: 0x${pc.toUInt().toString(16).uppercase()}", style = MaterialTheme.typography.bodySmall)
                Text("Ticks: $tickCount", style = MaterialTheme.typography.bodySmall)
                Text("EE State: $debugState", style = MaterialTheme.typography.bodySmall)
            }
        }
    }
}