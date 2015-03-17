<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmMain
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.btnConnect = New System.Windows.Forms.Button()
        Me.btnDisconnect = New System.Windows.Forms.Button()
        Me.cmbPort = New System.Windows.Forms.ComboBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.GroupBox1 = New System.Windows.Forms.GroupBox()
        Me.btnLedsOff = New System.Windows.Forms.Button()
        Me.btnTempClear = New System.Windows.Forms.Button()
        Me.btnAnalogClear = New System.Windows.Forms.Button()
        Me.txtTransmitCode = New System.Windows.Forms.TextBox()
        Me.btnAT = New System.Windows.Forms.Button()
        Me.btnAV = New System.Windows.Forms.Button()
        Me.btnTemp7 = New System.Windows.Forms.Button()
        Me.btnTemp6 = New System.Windows.Forms.Button()
        Me.btnLeds8 = New System.Windows.Forms.Button()
        Me.btnLeds7 = New System.Windows.Forms.Button()
        Me.btnLeds5 = New System.Windows.Forms.Button()
        Me.btnLeds4 = New System.Windows.Forms.Button()
        Me.grpNewAlarm = New System.Windows.Forms.GroupBox()
        Me.lblAutoRepeat = New System.Windows.Forms.Label()
        Me.lblTimerOrAlarm = New System.Windows.Forms.Label()
        Me.lblAlarmValues = New System.Windows.Forms.Label()
        Me.txtAlarmInput = New System.Windows.Forms.TextBox()
        Me.Label4 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.btnAlarm3on = New System.Windows.Forms.Button()
        Me.btnAlarm2on = New System.Windows.Forms.Button()
        Me.btnAlarm1on = New System.Windows.Forms.Button()
        Me.btnAlarm0on = New System.Windows.Forms.Button()
        Me.btnAlarm3off = New System.Windows.Forms.Button()
        Me.btnAlarm2off = New System.Windows.Forms.Button()
        Me.btnAlarm1off = New System.Windows.Forms.Button()
        Me.btnAlarm0off = New System.Windows.Forms.Button()
        Me.btnAlarm3 = New System.Windows.Forms.Button()
        Me.btnAlarm2 = New System.Windows.Forms.Button()
        Me.btnAlarm1 = New System.Windows.Forms.Button()
        Me.btnVersion = New System.Windows.Forms.Button()
        Me.btnAnalog8 = New System.Windows.Forms.Button()
        Me.btnAnalog7 = New System.Windows.Forms.Button()
        Me.btnAnalog6 = New System.Windows.Forms.Button()
        Me.btnAnalog3 = New System.Windows.Forms.Button()
        Me.btnAnalog2 = New System.Windows.Forms.Button()
        Me.btnAnalog1 = New System.Windows.Forms.Button()
        Me.btnAnalog0 = New System.Windows.Forms.Button()
        Me.btnBeep51 = New System.Windows.Forms.Button()
        Me.btnBeep1 = New System.Windows.Forms.Button()
        Me.btnCounter3 = New System.Windows.Forms.Button()
        Me.btnCounter2 = New System.Windows.Forms.Button()
        Me.btnLeds6 = New System.Windows.Forms.Button()
        Me.btnLeds3 = New System.Windows.Forms.Button()
        Me.btnLeds2 = New System.Windows.Forms.Button()
        Me.btnLeds1 = New System.Windows.Forms.Button()
        Me.btnTemp1 = New System.Windows.Forms.Button()
        Me.btnTemp0 = New System.Windows.Forms.Button()
        Me.lblMem = New System.Windows.Forms.Label()
        Me.lblQ = New System.Windows.Forms.Label()
        Me.lblSendCount = New System.Windows.Forms.Label()
        Me.btnTest = New System.Windows.Forms.Button()
        Me.btnRelay8 = New System.Windows.Forms.Button()
        Me.btnRelay7 = New System.Windows.Forms.Button()
        Me.btnRelay6 = New System.Windows.Forms.Button()
        Me.btnRelay5 = New System.Windows.Forms.Button()
        Me.btnRelay4 = New System.Windows.Forms.Button()
        Me.btnRelay3 = New System.Windows.Forms.Button()
        Me.btnRelay2 = New System.Windows.Forms.Button()
        Me.btnAlarm0 = New System.Windows.Forms.Button()
        Me.btnRelay1 = New System.Windows.Forms.Button()
        Me.btnTime = New System.Windows.Forms.Button()
        Me.btnDate = New System.Windows.Forms.Button()
        Me.btnMemory = New System.Windows.Forms.Button()
        Me.btnQuery = New System.Windows.Forms.Button()
        Me.btnSend = New System.Windows.Forms.Button()
        Me.txtTransmit = New System.Windows.Forms.TextBox()
        Me.GroupBox2 = New System.Windows.Forms.GroupBox()
        Me.lblReplyExpectedText = New System.Windows.Forms.Label()
        Me.lblReplyExpected = New System.Windows.Forms.Label()
        Me.lblReceiveCount = New System.Windows.Forms.Label()
        Me.btnClearReceived = New System.Windows.Forms.Button()
        Me.rtbReceived = New System.Windows.Forms.RichTextBox()
        Me.SerialPort1 = New System.IO.Ports.SerialPort(Me.components)
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.Timer2 = New System.Windows.Forms.Timer(Me.components)
        Me.lblDateTime = New System.Windows.Forms.Label()
        Me.txtDeviceId = New System.Windows.Forms.TextBox()
        Me.Label5 = New System.Windows.Forms.Label()
        Me.GroupBox1.SuspendLayout()
        Me.grpNewAlarm.SuspendLayout()
        Me.GroupBox2.SuspendLayout()
        Me.SuspendLayout()
        '
        'btnConnect
        '
        Me.btnConnect.Location = New System.Drawing.Point(245, 18)
        Me.btnConnect.Name = "btnConnect"
        Me.btnConnect.Size = New System.Drawing.Size(75, 23)
        Me.btnConnect.TabIndex = 0
        Me.btnConnect.Text = "Connect "
        Me.btnConnect.UseVisualStyleBackColor = True
        '
        'btnDisconnect
        '
        Me.btnDisconnect.Location = New System.Drawing.Point(326, 18)
        Me.btnDisconnect.Name = "btnDisconnect"
        Me.btnDisconnect.Size = New System.Drawing.Size(75, 23)
        Me.btnDisconnect.TabIndex = 0
        Me.btnDisconnect.Text = "Disconnect"
        Me.btnDisconnect.UseVisualStyleBackColor = True
        '
        'cmbPort
        '
        Me.cmbPort.FormattingEnabled = True
        Me.cmbPort.Location = New System.Drawing.Point(104, 20)
        Me.cmbPort.Name = "cmbPort"
        Me.cmbPort.Size = New System.Drawing.Size(121, 21)
        Me.cmbPort.TabIndex = 1
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(40, 23)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(58, 13)
        Me.Label1.TabIndex = 2
        Me.Label1.Text = "Comm Port"
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.btnLedsOff)
        Me.GroupBox1.Controls.Add(Me.btnTempClear)
        Me.GroupBox1.Controls.Add(Me.btnAnalogClear)
        Me.GroupBox1.Controls.Add(Me.txtTransmitCode)
        Me.GroupBox1.Controls.Add(Me.btnAT)
        Me.GroupBox1.Controls.Add(Me.btnAV)
        Me.GroupBox1.Controls.Add(Me.btnTemp7)
        Me.GroupBox1.Controls.Add(Me.btnTemp6)
        Me.GroupBox1.Controls.Add(Me.btnLeds8)
        Me.GroupBox1.Controls.Add(Me.btnLeds7)
        Me.GroupBox1.Controls.Add(Me.btnLeds5)
        Me.GroupBox1.Controls.Add(Me.btnLeds4)
        Me.GroupBox1.Controls.Add(Me.grpNewAlarm)
        Me.GroupBox1.Controls.Add(Me.Label4)
        Me.GroupBox1.Controls.Add(Me.Label3)
        Me.GroupBox1.Controls.Add(Me.Label2)
        Me.GroupBox1.Controls.Add(Me.btnAlarm3on)
        Me.GroupBox1.Controls.Add(Me.btnAlarm2on)
        Me.GroupBox1.Controls.Add(Me.btnAlarm1on)
        Me.GroupBox1.Controls.Add(Me.btnAlarm0on)
        Me.GroupBox1.Controls.Add(Me.btnAlarm3off)
        Me.GroupBox1.Controls.Add(Me.btnAlarm2off)
        Me.GroupBox1.Controls.Add(Me.btnAlarm1off)
        Me.GroupBox1.Controls.Add(Me.btnAlarm0off)
        Me.GroupBox1.Controls.Add(Me.btnAlarm3)
        Me.GroupBox1.Controls.Add(Me.btnAlarm2)
        Me.GroupBox1.Controls.Add(Me.btnAlarm1)
        Me.GroupBox1.Controls.Add(Me.btnVersion)
        Me.GroupBox1.Controls.Add(Me.btnAnalog8)
        Me.GroupBox1.Controls.Add(Me.btnAnalog7)
        Me.GroupBox1.Controls.Add(Me.btnAnalog6)
        Me.GroupBox1.Controls.Add(Me.btnAnalog3)
        Me.GroupBox1.Controls.Add(Me.btnAnalog2)
        Me.GroupBox1.Controls.Add(Me.btnAnalog1)
        Me.GroupBox1.Controls.Add(Me.btnAnalog0)
        Me.GroupBox1.Controls.Add(Me.btnBeep51)
        Me.GroupBox1.Controls.Add(Me.btnBeep1)
        Me.GroupBox1.Controls.Add(Me.btnCounter3)
        Me.GroupBox1.Controls.Add(Me.btnCounter2)
        Me.GroupBox1.Controls.Add(Me.btnLeds6)
        Me.GroupBox1.Controls.Add(Me.btnLeds3)
        Me.GroupBox1.Controls.Add(Me.btnLeds2)
        Me.GroupBox1.Controls.Add(Me.btnLeds1)
        Me.GroupBox1.Controls.Add(Me.btnTemp1)
        Me.GroupBox1.Controls.Add(Me.btnTemp0)
        Me.GroupBox1.Controls.Add(Me.lblMem)
        Me.GroupBox1.Controls.Add(Me.lblQ)
        Me.GroupBox1.Controls.Add(Me.lblSendCount)
        Me.GroupBox1.Controls.Add(Me.btnTest)
        Me.GroupBox1.Controls.Add(Me.btnRelay8)
        Me.GroupBox1.Controls.Add(Me.btnRelay7)
        Me.GroupBox1.Controls.Add(Me.btnRelay6)
        Me.GroupBox1.Controls.Add(Me.btnRelay5)
        Me.GroupBox1.Controls.Add(Me.btnRelay4)
        Me.GroupBox1.Controls.Add(Me.btnRelay3)
        Me.GroupBox1.Controls.Add(Me.btnRelay2)
        Me.GroupBox1.Controls.Add(Me.btnAlarm0)
        Me.GroupBox1.Controls.Add(Me.btnRelay1)
        Me.GroupBox1.Controls.Add(Me.btnTime)
        Me.GroupBox1.Controls.Add(Me.btnDate)
        Me.GroupBox1.Controls.Add(Me.btnMemory)
        Me.GroupBox1.Controls.Add(Me.btnQuery)
        Me.GroupBox1.Controls.Add(Me.btnSend)
        Me.GroupBox1.Controls.Add(Me.txtTransmit)
        Me.GroupBox1.Location = New System.Drawing.Point(26, 76)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(742, 346)
        Me.GroupBox1.TabIndex = 3
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "Transmit Data"
        '
        'btnLedsOff
        '
        Me.btnLedsOff.Location = New System.Drawing.Point(441, 146)
        Me.btnLedsOff.Name = "btnLedsOff"
        Me.btnLedsOff.Size = New System.Drawing.Size(31, 23)
        Me.btnLedsOff.TabIndex = 64
        Me.btnLedsOff.Text = "L:˅"
        Me.btnLedsOff.UseVisualStyleBackColor = True
        '
        'btnTempClear
        '
        Me.btnTempClear.Location = New System.Drawing.Point(293, 189)
        Me.btnTempClear.Name = "btnTempClear"
        Me.btnTempClear.Size = New System.Drawing.Size(40, 23)
        Me.btnTempClear.TabIndex = 63
        Me.btnTempClear.Text = "T0:0"
        Me.btnTempClear.UseVisualStyleBackColor = True
        '
        'btnAnalogClear
        '
        Me.btnAnalogClear.Location = New System.Drawing.Point(367, 189)
        Me.btnAnalogClear.Name = "btnAnalogClear"
        Me.btnAnalogClear.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalogClear.TabIndex = 62
        Me.btnAnalogClear.Text = "a0:"
        Me.btnAnalogClear.UseVisualStyleBackColor = True
        '
        'txtTransmitCode
        '
        Me.txtTransmitCode.Location = New System.Drawing.Point(17, 310)
        Me.txtTransmitCode.Name = "txtTransmitCode"
        Me.txtTransmitCode.Size = New System.Drawing.Size(381, 20)
        Me.txtTransmitCode.TabIndex = 61
        '
        'btnAT
        '
        Me.btnAT.Location = New System.Drawing.Point(330, 223)
        Me.btnAT.Name = "btnAT"
        Me.btnAT.Size = New System.Drawing.Size(31, 23)
        Me.btnAT.TabIndex = 60
        Me.btnAT.Text = "AT"
        Me.btnAT.UseVisualStyleBackColor = True
        '
        'btnAV
        '
        Me.btnAV.Location = New System.Drawing.Point(293, 223)
        Me.btnAV.Name = "btnAV"
        Me.btnAV.Size = New System.Drawing.Size(31, 23)
        Me.btnAV.TabIndex = 59
        Me.btnAV.Text = "AV"
        Me.btnAV.UseVisualStyleBackColor = True
        '
        'btnTemp7
        '
        Me.btnTemp7.Location = New System.Drawing.Point(256, 189)
        Me.btnTemp7.Name = "btnTemp7"
        Me.btnTemp7.Size = New System.Drawing.Size(31, 23)
        Me.btnTemp7.TabIndex = 58
        Me.btnTemp7.Text = "T7"
        Me.btnTemp7.UseVisualStyleBackColor = True
        '
        'btnTemp6
        '
        Me.btnTemp6.Location = New System.Drawing.Point(219, 189)
        Me.btnTemp6.Name = "btnTemp6"
        Me.btnTemp6.Size = New System.Drawing.Size(31, 23)
        Me.btnTemp6.TabIndex = 57
        Me.btnTemp6.Text = "T6"
        Me.btnTemp6.UseVisualStyleBackColor = True
        '
        'btnLeds8
        '
        Me.btnLeds8.Location = New System.Drawing.Point(404, 146)
        Me.btnLeds8.Name = "btnLeds8"
        Me.btnLeds8.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds8.TabIndex = 56
        Me.btnLeds8.Text = "L8:"
        Me.btnLeds8.UseVisualStyleBackColor = True
        '
        'btnLeds7
        '
        Me.btnLeds7.Location = New System.Drawing.Point(367, 146)
        Me.btnLeds7.Name = "btnLeds7"
        Me.btnLeds7.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds7.TabIndex = 55
        Me.btnLeds7.Text = "L7:"
        Me.btnLeds7.UseVisualStyleBackColor = True
        '
        'btnLeds5
        '
        Me.btnLeds5.Location = New System.Drawing.Point(293, 146)
        Me.btnLeds5.Name = "btnLeds5"
        Me.btnLeds5.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds5.TabIndex = 54
        Me.btnLeds5.Text = "L5:"
        Me.btnLeds5.UseVisualStyleBackColor = True
        '
        'btnLeds4
        '
        Me.btnLeds4.Location = New System.Drawing.Point(256, 146)
        Me.btnLeds4.Name = "btnLeds4"
        Me.btnLeds4.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds4.TabIndex = 53
        Me.btnLeds4.Text = "L4:"
        Me.btnLeds4.UseVisualStyleBackColor = True
        '
        'grpNewAlarm
        '
        Me.grpNewAlarm.Controls.Add(Me.lblAutoRepeat)
        Me.grpNewAlarm.Controls.Add(Me.lblTimerOrAlarm)
        Me.grpNewAlarm.Controls.Add(Me.lblAlarmValues)
        Me.grpNewAlarm.Controls.Add(Me.txtAlarmInput)
        Me.grpNewAlarm.Location = New System.Drawing.Point(491, 223)
        Me.grpNewAlarm.Name = "grpNewAlarm"
        Me.grpNewAlarm.Size = New System.Drawing.Size(214, 77)
        Me.grpNewAlarm.TabIndex = 52
        Me.grpNewAlarm.TabStop = False
        Me.grpNewAlarm.Text = "New Alarm"
        '
        'lblAutoRepeat
        '
        Me.lblAutoRepeat.AutoSize = True
        Me.lblAutoRepeat.Location = New System.Drawing.Point(5, 20)
        Me.lblAutoRepeat.Name = "lblAutoRepeat"
        Me.lblAutoRepeat.Size = New System.Drawing.Size(67, 13)
        Me.lblAutoRepeat.TabIndex = 54
        Me.lblAutoRepeat.Text = "Auto Repeat"
        '
        'lblTimerOrAlarm
        '
        Me.lblTimerOrAlarm.AutoSize = True
        Me.lblTimerOrAlarm.Location = New System.Drawing.Point(5, 51)
        Me.lblTimerOrAlarm.Name = "lblTimerOrAlarm"
        Me.lblTimerOrAlarm.Size = New System.Drawing.Size(33, 13)
        Me.lblTimerOrAlarm.TabIndex = 53
        Me.lblTimerOrAlarm.Text = "Timer"
        '
        'lblAlarmValues
        '
        Me.lblAlarmValues.AutoSize = True
        Me.lblAlarmValues.Location = New System.Drawing.Point(116, 20)
        Me.lblAlarmValues.Name = "lblAlarmValues"
        Me.lblAlarmValues.Size = New System.Drawing.Size(22, 13)
        Me.lblAlarmValues.TabIndex = 52
        Me.lblAlarmValues.Text = "0,0"
        '
        'txtAlarmInput
        '
        Me.txtAlarmInput.Location = New System.Drawing.Point(119, 48)
        Me.txtAlarmInput.Name = "txtAlarmInput"
        Me.txtAlarmInput.Size = New System.Drawing.Size(86, 20)
        Me.txtAlarmInput.TabIndex = 51
        '
        'Label4
        '
        Me.Label4.AutoSize = True
        Me.Label4.Location = New System.Drawing.Point(521, 194)
        Me.Label4.Name = "Label4"
        Me.Label4.Size = New System.Drawing.Size(19, 13)
        Me.Label4.TabIndex = 49
        Me.Label4.Text = "on"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(521, 151)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(19, 13)
        Me.Label3.TabIndex = 48
        Me.Label3.Text = "off"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(501, 107)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(39, 13)
        Me.Label2.TabIndex = 47
        Me.Label2.Text = "display"
        '
        'btnAlarm3on
        '
        Me.btnAlarm3on.Location = New System.Drawing.Point(657, 189)
        Me.btnAlarm3on.Name = "btnAlarm3on"
        Me.btnAlarm3on.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm3on.TabIndex = 46
        Me.btnAlarm3on.Text = "A3"
        Me.btnAlarm3on.UseVisualStyleBackColor = True
        '
        'btnAlarm2on
        '
        Me.btnAlarm2on.Location = New System.Drawing.Point(620, 189)
        Me.btnAlarm2on.Name = "btnAlarm2on"
        Me.btnAlarm2on.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm2on.TabIndex = 45
        Me.btnAlarm2on.Text = "A2"
        Me.btnAlarm2on.UseVisualStyleBackColor = True
        '
        'btnAlarm1on
        '
        Me.btnAlarm1on.Location = New System.Drawing.Point(583, 189)
        Me.btnAlarm1on.Name = "btnAlarm1on"
        Me.btnAlarm1on.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm1on.TabIndex = 44
        Me.btnAlarm1on.Text = "A1"
        Me.btnAlarm1on.UseVisualStyleBackColor = True
        '
        'btnAlarm0on
        '
        Me.btnAlarm0on.Location = New System.Drawing.Point(546, 189)
        Me.btnAlarm0on.Name = "btnAlarm0on"
        Me.btnAlarm0on.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm0on.TabIndex = 43
        Me.btnAlarm0on.Text = "A0"
        Me.btnAlarm0on.UseVisualStyleBackColor = True
        '
        'btnAlarm3off
        '
        Me.btnAlarm3off.Location = New System.Drawing.Point(657, 146)
        Me.btnAlarm3off.Name = "btnAlarm3off"
        Me.btnAlarm3off.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm3off.TabIndex = 42
        Me.btnAlarm3off.Text = "A3"
        Me.btnAlarm3off.UseVisualStyleBackColor = True
        '
        'btnAlarm2off
        '
        Me.btnAlarm2off.Location = New System.Drawing.Point(620, 146)
        Me.btnAlarm2off.Name = "btnAlarm2off"
        Me.btnAlarm2off.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm2off.TabIndex = 41
        Me.btnAlarm2off.Text = "A2"
        Me.btnAlarm2off.UseVisualStyleBackColor = True
        '
        'btnAlarm1off
        '
        Me.btnAlarm1off.Location = New System.Drawing.Point(583, 146)
        Me.btnAlarm1off.Name = "btnAlarm1off"
        Me.btnAlarm1off.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm1off.TabIndex = 40
        Me.btnAlarm1off.Text = "A1"
        Me.btnAlarm1off.UseVisualStyleBackColor = True
        '
        'btnAlarm0off
        '
        Me.btnAlarm0off.Location = New System.Drawing.Point(546, 146)
        Me.btnAlarm0off.Name = "btnAlarm0off"
        Me.btnAlarm0off.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm0off.TabIndex = 39
        Me.btnAlarm0off.Text = "A0"
        Me.btnAlarm0off.UseVisualStyleBackColor = True
        '
        'btnAlarm3
        '
        Me.btnAlarm3.Location = New System.Drawing.Point(657, 102)
        Me.btnAlarm3.Name = "btnAlarm3"
        Me.btnAlarm3.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm3.TabIndex = 38
        Me.btnAlarm3.Text = "A3"
        Me.btnAlarm3.UseVisualStyleBackColor = True
        '
        'btnAlarm2
        '
        Me.btnAlarm2.Location = New System.Drawing.Point(620, 102)
        Me.btnAlarm2.Name = "btnAlarm2"
        Me.btnAlarm2.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm2.TabIndex = 37
        Me.btnAlarm2.Text = "A2"
        Me.btnAlarm2.UseVisualStyleBackColor = True
        '
        'btnAlarm1
        '
        Me.btnAlarm1.Location = New System.Drawing.Point(583, 102)
        Me.btnAlarm1.Name = "btnAlarm1"
        Me.btnAlarm1.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm1.TabIndex = 36
        Me.btnAlarm1.Text = "A1"
        Me.btnAlarm1.UseVisualStyleBackColor = True
        '
        'btnVersion
        '
        Me.btnVersion.Location = New System.Drawing.Point(78, 132)
        Me.btnVersion.Name = "btnVersion"
        Me.btnVersion.Size = New System.Drawing.Size(21, 23)
        Me.btnVersion.TabIndex = 34
        Me.btnVersion.Text = "v"
        Me.btnVersion.UseVisualStyleBackColor = True
        '
        'btnAnalog8
        '
        Me.btnAnalog8.Location = New System.Drawing.Point(404, 189)
        Me.btnAnalog8.Name = "btnAnalog8"
        Me.btnAnalog8.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalog8.TabIndex = 33
        Me.btnAnalog8.Text = "a8"
        Me.btnAnalog8.UseVisualStyleBackColor = True
        '
        'btnAnalog7
        '
        Me.btnAnalog7.Location = New System.Drawing.Point(404, 223)
        Me.btnAnalog7.Name = "btnAnalog7"
        Me.btnAnalog7.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalog7.TabIndex = 32
        Me.btnAnalog7.Text = "a7"
        Me.btnAnalog7.UseVisualStyleBackColor = True
        '
        'btnAnalog6
        '
        Me.btnAnalog6.Location = New System.Drawing.Point(367, 223)
        Me.btnAnalog6.Name = "btnAnalog6"
        Me.btnAnalog6.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalog6.TabIndex = 31
        Me.btnAnalog6.Text = "a6"
        Me.btnAnalog6.UseVisualStyleBackColor = True
        '
        'btnAnalog3
        '
        Me.btnAnalog3.Location = New System.Drawing.Point(256, 223)
        Me.btnAnalog3.Name = "btnAnalog3"
        Me.btnAnalog3.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalog3.TabIndex = 30
        Me.btnAnalog3.Text = "a3"
        Me.btnAnalog3.UseVisualStyleBackColor = True
        '
        'btnAnalog2
        '
        Me.btnAnalog2.Location = New System.Drawing.Point(219, 223)
        Me.btnAnalog2.Name = "btnAnalog2"
        Me.btnAnalog2.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalog2.TabIndex = 29
        Me.btnAnalog2.Text = "a2"
        Me.btnAnalog2.UseVisualStyleBackColor = True
        '
        'btnAnalog1
        '
        Me.btnAnalog1.Location = New System.Drawing.Point(182, 223)
        Me.btnAnalog1.Name = "btnAnalog1"
        Me.btnAnalog1.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalog1.TabIndex = 28
        Me.btnAnalog1.Text = "a1"
        Me.btnAnalog1.UseVisualStyleBackColor = True
        '
        'btnAnalog0
        '
        Me.btnAnalog0.Location = New System.Drawing.Point(145, 223)
        Me.btnAnalog0.Name = "btnAnalog0"
        Me.btnAnalog0.Size = New System.Drawing.Size(31, 23)
        Me.btnAnalog0.TabIndex = 27
        Me.btnAnalog0.Text = "a0"
        Me.btnAnalog0.UseVisualStyleBackColor = True
        '
        'btnBeep51
        '
        Me.btnBeep51.Location = New System.Drawing.Point(293, 264)
        Me.btnBeep51.Name = "btnBeep51"
        Me.btnBeep51.Size = New System.Drawing.Size(40, 23)
        Me.btnBeep51.TabIndex = 26
        Me.btnBeep51.Text = "b51"
        Me.btnBeep51.UseVisualStyleBackColor = True
        '
        'btnBeep1
        '
        Me.btnBeep1.Location = New System.Drawing.Point(256, 264)
        Me.btnBeep1.Name = "btnBeep1"
        Me.btnBeep1.Size = New System.Drawing.Size(31, 23)
        Me.btnBeep1.TabIndex = 25
        Me.btnBeep1.Text = "b1"
        Me.btnBeep1.UseVisualStyleBackColor = True
        '
        'btnCounter3
        '
        Me.btnCounter3.Location = New System.Drawing.Point(182, 264)
        Me.btnCounter3.Name = "btnCounter3"
        Me.btnCounter3.Size = New System.Drawing.Size(31, 23)
        Me.btnCounter3.TabIndex = 24
        Me.btnCounter3.Text = "i1"
        Me.btnCounter3.UseVisualStyleBackColor = True
        '
        'btnCounter2
        '
        Me.btnCounter2.Location = New System.Drawing.Point(145, 264)
        Me.btnCounter2.Name = "btnCounter2"
        Me.btnCounter2.Size = New System.Drawing.Size(31, 23)
        Me.btnCounter2.TabIndex = 23
        Me.btnCounter2.Text = "i0"
        Me.btnCounter2.UseVisualStyleBackColor = True
        '
        'btnLeds6
        '
        Me.btnLeds6.Location = New System.Drawing.Point(330, 146)
        Me.btnLeds6.Name = "btnLeds6"
        Me.btnLeds6.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds6.TabIndex = 22
        Me.btnLeds6.Text = "L6:"
        Me.btnLeds6.UseVisualStyleBackColor = True
        '
        'btnLeds3
        '
        Me.btnLeds3.Location = New System.Drawing.Point(219, 146)
        Me.btnLeds3.Name = "btnLeds3"
        Me.btnLeds3.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds3.TabIndex = 21
        Me.btnLeds3.Text = "L3:"
        Me.btnLeds3.UseVisualStyleBackColor = True
        '
        'btnLeds2
        '
        Me.btnLeds2.Location = New System.Drawing.Point(182, 146)
        Me.btnLeds2.Name = "btnLeds2"
        Me.btnLeds2.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds2.TabIndex = 20
        Me.btnLeds2.Text = "L2:"
        Me.btnLeds2.UseVisualStyleBackColor = True
        '
        'btnLeds1
        '
        Me.btnLeds1.Location = New System.Drawing.Point(145, 146)
        Me.btnLeds1.Name = "btnLeds1"
        Me.btnLeds1.Size = New System.Drawing.Size(31, 23)
        Me.btnLeds1.TabIndex = 19
        Me.btnLeds1.Text = "L1:"
        Me.btnLeds1.UseVisualStyleBackColor = True
        '
        'btnTemp1
        '
        Me.btnTemp1.Location = New System.Drawing.Point(182, 189)
        Me.btnTemp1.Name = "btnTemp1"
        Me.btnTemp1.Size = New System.Drawing.Size(31, 23)
        Me.btnTemp1.TabIndex = 17
        Me.btnTemp1.Text = "T1"
        Me.btnTemp1.UseVisualStyleBackColor = True
        '
        'btnTemp0
        '
        Me.btnTemp0.Location = New System.Drawing.Point(145, 189)
        Me.btnTemp0.Name = "btnTemp0"
        Me.btnTemp0.Size = New System.Drawing.Size(31, 23)
        Me.btnTemp0.TabIndex = 16
        Me.btnTemp0.Text = "T0"
        Me.btnTemp0.UseVisualStyleBackColor = True
        '
        'lblMem
        '
        Me.lblMem.AutoSize = True
        Me.lblMem.Location = New System.Drawing.Point(48, 73)
        Me.lblMem.Name = "lblMem"
        Me.lblMem.Size = New System.Drawing.Size(13, 13)
        Me.lblMem.TabIndex = 15
        Me.lblMem.Text = "0"
        '
        'lblQ
        '
        Me.lblQ.AutoSize = True
        Me.lblQ.Location = New System.Drawing.Point(21, 73)
        Me.lblQ.Name = "lblQ"
        Me.lblQ.Size = New System.Drawing.Size(13, 13)
        Me.lblQ.TabIndex = 15
        Me.lblQ.Text = "0"
        '
        'lblSendCount
        '
        Me.lblSendCount.AutoSize = True
        Me.lblSendCount.Location = New System.Drawing.Point(496, 44)
        Me.lblSendCount.Name = "lblSendCount"
        Me.lblSendCount.Size = New System.Drawing.Size(13, 13)
        Me.lblSendCount.TabIndex = 14
        Me.lblSendCount.Text = "0"
        '
        'btnTest
        '
        Me.btnTest.Location = New System.Drawing.Point(650, 41)
        Me.btnTest.Name = "btnTest"
        Me.btnTest.Size = New System.Drawing.Size(38, 23)
        Me.btnTest.TabIndex = 13
        Me.btnTest.Text = "Test"
        Me.btnTest.UseVisualStyleBackColor = True
        '
        'btnRelay8
        '
        Me.btnRelay8.Location = New System.Drawing.Point(404, 102)
        Me.btnRelay8.Name = "btnRelay8"
        Me.btnRelay8.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay8.TabIndex = 13
        Me.btnRelay8.Text = "R8"
        Me.btnRelay8.UseVisualStyleBackColor = True
        '
        'btnRelay7
        '
        Me.btnRelay7.Location = New System.Drawing.Point(367, 102)
        Me.btnRelay7.Name = "btnRelay7"
        Me.btnRelay7.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay7.TabIndex = 12
        Me.btnRelay7.Text = "R7"
        Me.btnRelay7.UseVisualStyleBackColor = True
        '
        'btnRelay6
        '
        Me.btnRelay6.Location = New System.Drawing.Point(330, 102)
        Me.btnRelay6.Name = "btnRelay6"
        Me.btnRelay6.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay6.TabIndex = 11
        Me.btnRelay6.Text = "R6"
        Me.btnRelay6.UseVisualStyleBackColor = True
        '
        'btnRelay5
        '
        Me.btnRelay5.Location = New System.Drawing.Point(293, 102)
        Me.btnRelay5.Name = "btnRelay5"
        Me.btnRelay5.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay5.TabIndex = 10
        Me.btnRelay5.Text = "R5"
        Me.btnRelay5.UseVisualStyleBackColor = True
        '
        'btnRelay4
        '
        Me.btnRelay4.Location = New System.Drawing.Point(256, 102)
        Me.btnRelay4.Name = "btnRelay4"
        Me.btnRelay4.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay4.TabIndex = 9
        Me.btnRelay4.Text = "R4"
        Me.btnRelay4.UseVisualStyleBackColor = True
        '
        'btnRelay3
        '
        Me.btnRelay3.Location = New System.Drawing.Point(219, 102)
        Me.btnRelay3.Name = "btnRelay3"
        Me.btnRelay3.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay3.TabIndex = 8
        Me.btnRelay3.Text = "R3"
        Me.btnRelay3.UseVisualStyleBackColor = True
        '
        'btnRelay2
        '
        Me.btnRelay2.Location = New System.Drawing.Point(182, 102)
        Me.btnRelay2.Name = "btnRelay2"
        Me.btnRelay2.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay2.TabIndex = 7
        Me.btnRelay2.Text = "R2"
        Me.btnRelay2.UseVisualStyleBackColor = True
        '
        'btnAlarm0
        '
        Me.btnAlarm0.Location = New System.Drawing.Point(546, 102)
        Me.btnAlarm0.Name = "btnAlarm0"
        Me.btnAlarm0.Size = New System.Drawing.Size(31, 23)
        Me.btnAlarm0.TabIndex = 6
        Me.btnAlarm0.Text = "A0"
        Me.btnAlarm0.UseVisualStyleBackColor = True
        '
        'btnRelay1
        '
        Me.btnRelay1.Location = New System.Drawing.Point(145, 102)
        Me.btnRelay1.Name = "btnRelay1"
        Me.btnRelay1.Size = New System.Drawing.Size(31, 23)
        Me.btnRelay1.TabIndex = 6
        Me.btnRelay1.Text = "R1"
        Me.btnRelay1.UseVisualStyleBackColor = True
        '
        'btnTime
        '
        Me.btnTime.Location = New System.Drawing.Point(51, 102)
        Me.btnTime.Name = "btnTime"
        Me.btnTime.Size = New System.Drawing.Size(21, 23)
        Me.btnTime.TabIndex = 5
        Me.btnTime.Text = "t"
        Me.btnTime.UseVisualStyleBackColor = True
        '
        'btnDate
        '
        Me.btnDate.Location = New System.Drawing.Point(24, 102)
        Me.btnDate.Name = "btnDate"
        Me.btnDate.Size = New System.Drawing.Size(21, 23)
        Me.btnDate.TabIndex = 4
        Me.btnDate.Text = "d"
        Me.btnDate.UseVisualStyleBackColor = True
        '
        'btnMemory
        '
        Me.btnMemory.Location = New System.Drawing.Point(51, 132)
        Me.btnMemory.Name = "btnMemory"
        Me.btnMemory.Size = New System.Drawing.Size(21, 23)
        Me.btnMemory.TabIndex = 3
        Me.btnMemory.Text = "m"
        Me.btnMemory.UseVisualStyleBackColor = True
        '
        'btnQuery
        '
        Me.btnQuery.Location = New System.Drawing.Point(24, 132)
        Me.btnQuery.Name = "btnQuery"
        Me.btnQuery.Size = New System.Drawing.Size(21, 23)
        Me.btnQuery.TabIndex = 2
        Me.btnQuery.Text = "q"
        Me.btnQuery.UseVisualStyleBackColor = True
        '
        'btnSend
        '
        Me.btnSend.Location = New System.Drawing.Point(408, 41)
        Me.btnSend.Name = "btnSend"
        Me.btnSend.Size = New System.Drawing.Size(75, 23)
        Me.btnSend.TabIndex = 1
        Me.btnSend.Text = "Send"
        Me.btnSend.UseVisualStyleBackColor = True
        '
        'txtTransmit
        '
        Me.txtTransmit.Location = New System.Drawing.Point(24, 41)
        Me.txtTransmit.Name = "txtTransmit"
        Me.txtTransmit.Size = New System.Drawing.Size(374, 20)
        Me.txtTransmit.TabIndex = 0
        '
        'GroupBox2
        '
        Me.GroupBox2.Controls.Add(Me.lblReplyExpectedText)
        Me.GroupBox2.Controls.Add(Me.lblReplyExpected)
        Me.GroupBox2.Controls.Add(Me.lblReceiveCount)
        Me.GroupBox2.Controls.Add(Me.btnClearReceived)
        Me.GroupBox2.Controls.Add(Me.rtbReceived)
        Me.GroupBox2.Location = New System.Drawing.Point(26, 428)
        Me.GroupBox2.Name = "GroupBox2"
        Me.GroupBox2.Size = New System.Drawing.Size(742, 132)
        Me.GroupBox2.TabIndex = 4
        Me.GroupBox2.TabStop = False
        Me.GroupBox2.Text = "Received Data"
        '
        'lblReplyExpectedText
        '
        Me.lblReplyExpectedText.AutoSize = True
        Me.lblReplyExpectedText.Location = New System.Drawing.Point(647, 68)
        Me.lblReplyExpectedText.Name = "lblReplyExpectedText"
        Me.lblReplyExpectedText.Size = New System.Drawing.Size(82, 13)
        Me.lblReplyExpectedText.TabIndex = 4
        Me.lblReplyExpectedText.Text = "Reply Expected"
        '
        'lblReplyExpected
        '
        Me.lblReplyExpected.AutoSize = True
        Me.lblReplyExpected.Location = New System.Drawing.Point(607, 68)
        Me.lblReplyExpected.Name = "lblReplyExpected"
        Me.lblReplyExpected.Size = New System.Drawing.Size(13, 13)
        Me.lblReplyExpected.TabIndex = 3
        Me.lblReplyExpected.Text = "0"
        '
        'lblReceiveCount
        '
        Me.lblReceiveCount.AutoSize = True
        Me.lblReceiveCount.Location = New System.Drawing.Point(607, 29)
        Me.lblReceiveCount.Name = "lblReceiveCount"
        Me.lblReceiveCount.Size = New System.Drawing.Size(13, 13)
        Me.lblReceiveCount.TabIndex = 2
        Me.lblReceiveCount.Text = "0"
        '
        'btnClearReceived
        '
        Me.btnClearReceived.Location = New System.Drawing.Point(650, 19)
        Me.btnClearReceived.Name = "btnClearReceived"
        Me.btnClearReceived.Size = New System.Drawing.Size(75, 23)
        Me.btnClearReceived.TabIndex = 1
        Me.btnClearReceived.Text = "Clear"
        Me.btnClearReceived.UseVisualStyleBackColor = True
        '
        'rtbReceived
        '
        Me.rtbReceived.Location = New System.Drawing.Point(24, 19)
        Me.rtbReceived.Name = "rtbReceived"
        Me.rtbReceived.Size = New System.Drawing.Size(577, 100)
        Me.rtbReceived.TabIndex = 0
        Me.rtbReceived.Text = ""
        '
        'SerialPort1
        '
        '
        'Timer1
        '
        Me.Timer1.Enabled = True
        Me.Timer1.Interval = 60000
        '
        'Timer2
        '
        Me.Timer2.Enabled = True
        Me.Timer2.Interval = 1000
        '
        'lblDateTime
        '
        Me.lblDateTime.AutoSize = True
        Me.lblDateTime.Location = New System.Drawing.Point(592, 33)
        Me.lblDateTime.Name = "lblDateTime"
        Me.lblDateTime.Size = New System.Drawing.Size(81, 13)
        Me.lblDateTime.TabIndex = 15
        Me.lblDateTime.Text = "10/11 22:32:12"
        '
        'txtDeviceId
        '
        Me.txtDeviceId.Location = New System.Drawing.Point(104, 50)
        Me.txtDeviceId.Name = "txtDeviceId"
        Me.txtDeviceId.Size = New System.Drawing.Size(121, 20)
        Me.txtDeviceId.TabIndex = 16
        '
        'Label5
        '
        Me.Label5.AutoSize = True
        Me.Label5.Location = New System.Drawing.Point(40, 53)
        Me.Label5.Name = "Label5"
        Me.Label5.Size = New System.Drawing.Size(51, 13)
        Me.Label5.TabIndex = 17
        Me.Label5.Text = "RS485 id"
        '
        'frmMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(793, 567)
        Me.Controls.Add(Me.Label5)
        Me.Controls.Add(Me.txtDeviceId)
        Me.Controls.Add(Me.GroupBox2)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.cmbPort)
        Me.Controls.Add(Me.btnDisconnect)
        Me.Controls.Add(Me.btnConnect)
        Me.Controls.Add(Me.lblDateTime)
        Me.Name = "frmMain"
        Me.Text = "Arduino EEP"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        Me.grpNewAlarm.ResumeLayout(False)
        Me.grpNewAlarm.PerformLayout()
        Me.GroupBox2.ResumeLayout(False)
        Me.GroupBox2.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents btnConnect As System.Windows.Forms.Button
    Friend WithEvents btnDisconnect As System.Windows.Forms.Button
    Friend WithEvents cmbPort As System.Windows.Forms.ComboBox
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents btnSend As System.Windows.Forms.Button
    Friend WithEvents txtTransmit As System.Windows.Forms.TextBox
    Friend WithEvents GroupBox2 As System.Windows.Forms.GroupBox
    Friend WithEvents rtbReceived As System.Windows.Forms.RichTextBox
    Friend WithEvents SerialPort1 As System.IO.Ports.SerialPort
    Friend WithEvents btnClearReceived As System.Windows.Forms.Button
    Friend WithEvents btnTime As System.Windows.Forms.Button
    Friend WithEvents btnDate As System.Windows.Forms.Button
    Friend WithEvents btnMemory As System.Windows.Forms.Button
    Friend WithEvents btnQuery As System.Windows.Forms.Button
    Friend WithEvents btnRelay8 As System.Windows.Forms.Button
    Friend WithEvents btnRelay7 As System.Windows.Forms.Button
    Friend WithEvents btnRelay6 As System.Windows.Forms.Button
    Friend WithEvents btnRelay5 As System.Windows.Forms.Button
    Friend WithEvents btnRelay4 As System.Windows.Forms.Button
    Friend WithEvents btnRelay3 As System.Windows.Forms.Button
    Friend WithEvents btnRelay2 As System.Windows.Forms.Button
    Friend WithEvents btnRelay1 As System.Windows.Forms.Button
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents lblSendCount As System.Windows.Forms.Label
    Friend WithEvents lblReceiveCount As System.Windows.Forms.Label
    Friend WithEvents lblQ As System.Windows.Forms.Label
    Friend WithEvents lblMem As System.Windows.Forms.Label
    Friend WithEvents btnTest As System.Windows.Forms.Button
    Friend WithEvents btnLeds6 As System.Windows.Forms.Button
    Friend WithEvents btnLeds3 As System.Windows.Forms.Button
    Friend WithEvents btnLeds2 As System.Windows.Forms.Button
    Friend WithEvents btnLeds1 As System.Windows.Forms.Button
    Friend WithEvents btnTemp1 As System.Windows.Forms.Button
    Friend WithEvents btnTemp0 As System.Windows.Forms.Button
    Friend WithEvents btnBeep51 As System.Windows.Forms.Button
    Friend WithEvents btnBeep1 As System.Windows.Forms.Button
    Friend WithEvents btnCounter3 As System.Windows.Forms.Button
    Friend WithEvents btnCounter2 As System.Windows.Forms.Button
    Friend WithEvents btnAnalog8 As System.Windows.Forms.Button
    Friend WithEvents btnAnalog7 As System.Windows.Forms.Button
    Friend WithEvents btnAnalog6 As System.Windows.Forms.Button
    Friend WithEvents btnAnalog3 As System.Windows.Forms.Button
    Friend WithEvents btnAnalog2 As System.Windows.Forms.Button
    Friend WithEvents btnAnalog1 As System.Windows.Forms.Button
    Friend WithEvents btnAnalog0 As System.Windows.Forms.Button
    Friend WithEvents btnVersion As System.Windows.Forms.Button
    Friend WithEvents btnAlarm3 As System.Windows.Forms.Button
    Friend WithEvents btnAlarm2 As System.Windows.Forms.Button
    Friend WithEvents btnAlarm1 As System.Windows.Forms.Button
    Friend WithEvents btnAlarm0 As System.Windows.Forms.Button
    Friend WithEvents Label4 As System.Windows.Forms.Label
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
    Friend WithEvents btnAlarm3on As System.Windows.Forms.Button
    Friend WithEvents btnAlarm2on As System.Windows.Forms.Button
    Friend WithEvents btnAlarm1on As System.Windows.Forms.Button
    Friend WithEvents btnAlarm0on As System.Windows.Forms.Button
    Friend WithEvents btnAlarm3off As System.Windows.Forms.Button
    Friend WithEvents btnAlarm2off As System.Windows.Forms.Button
    Friend WithEvents btnAlarm1off As System.Windows.Forms.Button
    Friend WithEvents btnAlarm0off As System.Windows.Forms.Button
    Friend WithEvents Timer2 As System.Windows.Forms.Timer
    Friend WithEvents lblDateTime As System.Windows.Forms.Label
    Friend WithEvents grpNewAlarm As System.Windows.Forms.GroupBox
    Friend WithEvents lblAlarmValues As System.Windows.Forms.Label
    Friend WithEvents txtAlarmInput As System.Windows.Forms.TextBox
    Friend WithEvents lblTimerOrAlarm As System.Windows.Forms.Label
    Friend WithEvents lblAutoRepeat As System.Windows.Forms.Label
    Friend WithEvents btnLeds7 As System.Windows.Forms.Button
    Friend WithEvents btnLeds5 As System.Windows.Forms.Button
    Friend WithEvents btnLeds4 As System.Windows.Forms.Button
    Friend WithEvents btnLeds8 As System.Windows.Forms.Button
    Friend WithEvents btnTemp7 As System.Windows.Forms.Button
    Friend WithEvents btnTemp6 As System.Windows.Forms.Button
    Friend WithEvents txtDeviceId As System.Windows.Forms.TextBox
    Friend WithEvents Label5 As System.Windows.Forms.Label
    Friend WithEvents btnAT As System.Windows.Forms.Button
    Friend WithEvents btnAV As System.Windows.Forms.Button
    Friend WithEvents lblReplyExpectedText As System.Windows.Forms.Label
    Friend WithEvents lblReplyExpected As System.Windows.Forms.Label
    Friend WithEvents txtTransmitCode As System.Windows.Forms.TextBox
    Friend WithEvents btnTempClear As System.Windows.Forms.Button
    Friend WithEvents btnAnalogClear As System.Windows.Forms.Button
    Friend WithEvents btnLedsOff As System.Windows.Forms.Button

End Class
