package com.maxrblx1.sandboxsx2

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import com.maxrblx1.sandboxsx2.ui.theme.SandboxSX2Theme
import java.io.File

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Auto-create BIOS folders
        createBiosFolders()

        // Scan and register BIOS files
        scanAndRegisterBios()

        setContent {
            SandboxSX2Theme {
                Surface(modifier = Modifier.fillMaxSize()) {
                    EmulatorScreen(
                        onRescanBios = { scanAndRegisterBios() },
                        getBiosStatus = { getBiosStatus() }
                    )
                }
            }
        }
    }

    private fun createBiosFolders() {
        val root = File(getExternalFilesDir(null), "SandboxSX2")
        val folders = listOf("bin", "mec", "nvm", "erom", "roms")
        folders.forEach { name ->
            val dir = File(root, name)
            if (!dir.exists()) dir.mkdirs()
        }
    }

    private fun scanAndRegisterBios(): Int {
        val root = File(getExternalFilesDir(null), "SandboxSX2")
        val extensions = mapOf(
            ".bin" to "ROM",
            ".rom" to "ROM",
            ".rom1" to "ROM1",
            ".rom2" to "ROM2",
            ".erom" to "EROM",
            ".nvm" to "NVM",
            ".mec" to "MEC"
        )

        var count = 0

        root.walkTopDown().forEach { file ->
            if (file.isFile) {
                val nameLower = file.name.lowercase()
                var kind: String? = null

                // Detect by extension
                extensions.forEach { (ext, k) ->
                    if (nameLower.endsWith(ext)) {
                        kind = k
                    }
                }

                // Detect by parent folder if no extension match
                if (kind == null) {
                    val parentName = file.parentFile?.name?.lowercase()
                    kind = when (parentName) {
                        "bin", "roms" -> "ROM"
                        "erom" -> "EROM"
                        "mec" -> "MEC"
                        "nvm" -> "NVM"
                        else -> null
                    }
                }

                // Load BIOS if kind was determined
                kind?.let {
                    val bytes = file.readBytes()
                    NativeEmulator.loadBiosPart(it.uppercase(), bytes)
                    count++
                }
            }
        }
        return count
    }

    private fun getBiosStatus(): Map<String, Boolean> {
        val root = File(getExternalFilesDir(null), "SandboxSX2")
        val parts = mapOf(
            "ROM" to listOf(".bin", ".rom"),
            "ROM1" to listOf(".rom1"),
            "ROM2" to listOf(".rom2"),
            "EROM" to listOf(".erom"),
            "NVM" to listOf(".nvm"),
            "MEC" to listOf(".mec")
        )

        val status = mutableMapOf<String, Boolean>()
        parts.forEach { (part, exts) ->
            val found = root.walkTopDown().any { file ->
                file.isFile && exts.any { ext -> file.name.lowercase().endsWith(ext) }
            }
            status[part] = found
        }
        return status
    }
}

@Composable
fun EmulatorScreen(onRescanBios: () -> Int, getBiosStatus: () -> Map<String, Boolean>) {
    var debugText by remember { mutableStateOf("SandboxSX2 v0.3\nReady to debug...") }
    var showBiosStatus by remember { mutableStateOf(false) }
    val scrollState = rememberScrollState()

    Column(
        modifier = Modifier
            .padding(16.dp)
            .verticalScroll(scrollState)
    ) {
        Button(onClick = {
            val detected = onRescanBios()
            if (detected > 0) {
                NativeEmulator.initCore()
                debugText = "Core initialized. Detected $detected BIOS file(s)."
            } else {
                debugText = "Core initialization failed — ROM not found."
            }
        }) {
            Text("Init Core")
        }

        Spacer(modifier = Modifier.height(8.dp))

        Button(onClick = {
            debugText = if (NativeEmulator.isDebugReady()) {
                NativeEmulator.getDebugState()
            } else {
                "Debug not ready — load BIOS first."
            }
        }) {
            Text("Refresh Debug Info")
        }

        Spacer(modifier = Modifier.height(8.dp))

        Button(onClick = {
            if (NativeEmulator.isDebugReady()) {
                NativeEmulator.step()
                debugText = NativeEmulator.getDebugState()
            } else {
                debugText = "Debug not ready — initialize core and load BIOS first."
            }
        }) {
            Text("Step CPU")
        }

        Spacer(modifier = Modifier.height(8.dp))

        Button(onClick = {
            val status = getBiosStatus()
            val romReady = status["ROM"] == true
            debugText = if (NativeEmulator.isDebugReady() && romReady) {
                "✅ Emulator is ready for debugging."
            } else {
                "❌ Emulator not ready — ROM missing or core not initialized."
            }
        }) {
            Text("Check Debug Ready")
        }

        Spacer(modifier = Modifier.height(8.dp))

        Button(onClick = {
            val detected = onRescanBios()
            debugText = "Rescanned BIOS folders. Detected $detected BIOS file(s)."
        }) {
            Text("Rescan BIOS")
        }

        Spacer(modifier = Modifier.height(16.dp))

        // Toggle BIOS status
        Row(verticalAlignment = androidx.compose.ui.Alignment.CenterVertically) {
            Switch(
                checked = showBiosStatus,
                onCheckedChange = { showBiosStatus = it }
            )
            Spacer(modifier = Modifier.width(8.dp))
            Text("Show BIOS Parts Status")
        }

        if (showBiosStatus) {
            Spacer(modifier = Modifier.height(8.dp))
            BiosStatusView(getBiosStatus())
        }

        Spacer(modifier = Modifier.height(16.dp))
        Text(debugText)

        Spacer(modifier = Modifier.height(24.dp))

        // 🔔 Friendly warning message
        Text(
            "⚠️ Note: ROM is required for SandboxSX2 to run. " +
                    "Other BIOS parts (ROM1, ROM2, EROM, NVM, MEC) are optional, " +
                    "but missing files may cause reduced compatibility or features. " +
                    "If the emulator halts, please check your BIOS setup.",
            style = MaterialTheme.typography.bodyMedium
        )

        Spacer(modifier = Modifier.height(12.dp))

        // 💡 Helpful tip for v0.3
        Text(
            "💡 Tip: After adding or reloading BIOS files, always tap Init Core. " +
                    "This refreshes the emulator state so the BIOS is recognized and memory map is ready.",
            style = MaterialTheme.typography.bodyMedium
        )
    }
}

@Composable
fun BiosStatusView(status: Map<String, Boolean>) {
    val required = listOf("ROM")
    val optional = listOf("ROM1", "ROM2", "EROM", "NVM", "MEC")

    Column {
        Text("Required BIOS Parts:", style = MaterialTheme.typography.titleMedium)
        required.forEach { part ->
            val ok = status[part] == true
            Text("• $part ${if (ok) "✅" else "❌"}")
        }

        Spacer(modifier = Modifier.height(8.dp))

        Text("Optional BIOS Parts:", style = MaterialTheme.typography.titleMedium)
        optional.forEach { part ->
            val ok = status[part] == true
            Text("• $part ${if (ok) "✅" else "❌"}")
        }
    }
}