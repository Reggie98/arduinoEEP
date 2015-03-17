'Code Starts here ….
'Import Systems which we are gonna use in our code
Imports System
Imports System.ComponentModel
Imports System.Threading
Imports System.IO.Ports

'frmMain is the name of our form ….
'Here starts our main form code …..
Public Class frmMain
    Dim myPort As Array
    Dim relayStatus(8) As Boolean
    Dim relayPort() As String = {"16", "17", "14", "15", "33:0", "33:1", "33:2", "33:3"}
    Dim sendCount As Integer = 0
    Dim deviceId As Integer = 0
    Delegate Sub SetTextCallback(ByVal [text] As String)



    'Page Load Code Starts Here….
    Private Sub frmMain_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        myPort = IO.Ports.SerialPort.GetPortNames()
        For i = 0 To UBound(myPort)
            If (i > 1) Then
                cmbPort.Items.Add(myPort(i))
            End If
        Next
        cmbPort.Text = cmbPort.Items.Item(0)
        btnDisconnect.Enabled = False
        btnSend.Enabled = False

        For i = 0 To 7
            relayStatus(i) = False
        Next
    End Sub
    'Page Load Code Ends Here ….

    'Connect Button Code Starts Here ….
    Private Sub btnConnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnConnect.Click
        SerialPort1.PortName = cmbPort.Text
        SerialPort1.BaudRate = 115200
        SerialPort1.Handshake = Handshake.None
        SerialPort1.Parity = IO.Ports.Parity.None
        SerialPort1.StopBits = IO.Ports.StopBits.One
        SerialPort1.DataBits = 8
        SerialPort1.Open()
        btnConnect.Enabled = False
        cmbPort.Enabled = False
        btnDisconnect.Enabled = True
        btnSend.Enabled = True
        If (cmbPort.Text = "COM5" Or cmbPort.Text = "COM3") Then
            deviceId = 5
            txtDeviceId.Text = deviceId
            btnRelay3.Enabled = False
            btnRelay4.Enabled = False
            btnRelay5.Enabled = False
            btnRelay6.Enabled = False
            btnRelay7.Enabled = False
            btnRelay8.Enabled = False
            btnTemp0.Enabled = False
            btnTemp1.Enabled = False
            btnLeds1.Enabled = False
            btnLeds2.Enabled = False
            btnLeds3.Enabled = False
            btnLeds6.Enabled = False
            btnAnalog6.Enabled = False
            btnAnalog7.Enabled = False

        Else
            deviceId = 4
            txtDeviceId.Text = deviceId
            btnRelay3.Enabled = True
            btnRelay4.Enabled = True
            btnRelay5.Enabled = True
            btnRelay6.Enabled = True
            btnRelay7.Enabled = True
            btnRelay8.Enabled = True
            btnTemp0.Enabled = True
            btnTemp1.Enabled = True
            btnLeds1.Enabled = True
            btnLeds2.Enabled = True
            btnLeds3.Enabled = True
            btnLeds6.Enabled = True
            btnAnalog6.Enabled = True
            btnAnalog7.Enabled = True
        End If
    End Sub
    'Connect Button Code Ends Here ….

    'Disconnect Button Code Starts Here ….
    Private Sub btnDisconnect_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnDisconnect.Click
        SerialPort1.Close()
        btnConnect.Enabled = True
        btnDisconnect.Enabled = False
        btnSend.Enabled = False
        cmbPort.Enabled = True
    End Sub
    'Disconnect Button Code Ends Here ….

    Private Sub sendString(ByVal sendStr As String)
        Dim newStr As String
        Dim colonPosn As Integer
        If (sendStr = "t" And txtTransmit.Text <> "t") Then
            sendStr = Format(Now, "\tH:m:s")
        ElseIf (sendStr = "d") Then
            sendStr = Format(Now, "\dy:M:d")
        ElseIf (Mid(sendStr, 1, 1) = "w") Then
            colonPosn = InStr(sendStr, ":")
            newStr = Mid(sendStr, 1, colonPosn)
            For i = colonPosn + 1 To Math.Min(sendStr.Length, 32)
                If (i > colonPosn + 1) Then
                    newStr += ","
                End If
                newStr += Asc(Mid(sendStr, i, 1)).ToString
            Next
            sendStr = newStr
        End If
        sendStr = appendCRC8(sendStr)
        If SerialPort1.IsOpen Then
            SerialPort1.Write(sendStr)
        End If
        txtTransmitCode.Text = sendStr
        txtTransmitCode.Text = sendStr
        txtTransmitCode.Text = sendStr
    End Sub


    'Send Button Code Starts Here ….
    Private Sub btnSend_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnSend.Click
        sendString(txtTransmit.Text)
    End Sub
    'Send Button Code Ends Here ….

    Function calcCRC8(s As String) As Byte
        ' calculate the CRC8 for a string
        Dim j As Byte
        Dim k As Byte
        Dim crc8 As Byte
        Dim x As Byte
        crc8 = 0
        For m = 1 To Len(s)
            x = Asc(Mid(s, m, 1))
            For k = 0 To 7
                j = 1 And (x Xor crc8)
                crc8 = Fix(crc8 / 2) And &HFF
                x = Fix(x / 2) And &HFF
                If j <> 0 Then
                    crc8 = crc8 Xor &H8C
                End If
            Next k
        Next
        calcCRC8 = crc8
    End Function


    Private Function appendCRC8(ByVal str As String) As String
        ' append the CRC8 of a string to the end of the new string
        Dim crc8 As Byte
        Dim newStr As String
        If ((Mid(str, 1, 1) = "t" And txtTransmit.Text <> "t") Or Mid(str, 1, 1) = "d") Then
            newStr = "@00" + str + "c"          ' use the general deviceId
        Else
            newStr = "@0" + deviceId.ToString().Trim() + str + "c"
            lblSendCount.Text = Val(lblSendCount.Text) + 1
            lblReplyExpected.Text = Val(lblReplyExpected.Text) + 1
        End If

        crc8 = calcCRC8(newStr)
        If (crc8 < 100) Then
            newStr += "0"
        End If
        If (crc8 < 10) Then
            newStr += "0"
        End If
        newStr += crc8.ToString() & vbCrLf
        Return newStr
    End Function


    'Serial Port Receiving Code Starts Here ….
    Private Sub SerialPort1_DataReceived(ByVal sender As Object, ByVal e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort1.DataReceived
        ReceivedText(SerialPort1.ReadExisting())
    End Sub
    'Serial Port Receiving Code Ends Here ….

    'Serial Port Receiving Code(Invoke) Starts Here ….
    Private Sub ReceivedText(ByVal [text] As String)
        If Me.rtbReceived.InvokeRequired Then
            Dim x As New SetTextCallback(AddressOf ReceivedText)
            If [text].Length > 0 Then
                Me.Invoke(x, New Object() {(text)})
            End If
        Else
            Me.rtbReceived.Text &= [text]
            If [text].IndexOf("c") > 4 Then
                lblQ.Text = Mid([text], 4, 1)
                If lblQ.Text <> "K" Then
                End If
                If [text].IndexOf("c") > 5 Then
                    lblMem.Text = Mid([text], 5, [text].IndexOf("c") - 4)
                ElseIf [text].IndexOf("c") = 5 Then
                    lblMem.Text = Mid([text], 5, 1)
                Else
                    lblMem.Text = ""
                End If
                If lblQ.Text = "k" Then
                    ' sendString("w14:Keypad:" & lblMem.Text & " ")
                    If lblMem.Text = "15:0" Then
                        sendString("w0:Interactive   ")
                        sendString("w14: mode        ")
                        sendString("w28: menu        ")
                    End If
                ElseIf lblQ.Text = "P" Then
                    ' sendString("w28:PIR:" & lblMem.Text & "   ")
                End If
                lblReplyExpected.Text = Val(lblReplyExpected.Text) - 1
            End If
            Me.rtbReceived.SelectionStart = Me.rtbReceived.Text.Length
            Me.rtbReceived.ScrollToCaret()
            lblReceiveCount.Text = Val(lblReceiveCount.Text) + 1
        End If
    End Sub
    'Serial Port Receiving Code(Invoke) Ends Here ….

    'Com Port Change Warning Code Starts Here ….
    Private Sub cmbPort_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmbPort.SelectedIndexChanged
        If SerialPort1.IsOpen = False Then
            SerialPort1.PortName = cmbPort.Text
        Else
            MsgBox("Valid only if port is Closed", vbCritical)
        End If
    End Sub
    'Com Port Change Warning Code Ends Here ….


    Private Sub btnClearReceived_Click(sender As Object, e As EventArgs) Handles btnClearReceived.Click
        Me.rtbReceived.Text = ""
        lblSendCount.Text = 0
        lblReceiveCount.Text = 0
        lblReplyExpected.Text = 0
        txtTransmit.Text = ""
        lblMem.Text = ""
        lblQ.Text = ""
        txtTransmitCode.Text = ""
    End Sub

    Private Sub btnQ_Click(sender As Object, e As EventArgs) Handles btnQuery.Click
        sendString("q")
    End Sub

    Private Sub btnM_Click(sender As Object, e As EventArgs) Handles btnMemory.Click
        sendString("m")
    End Sub

    Private Sub btnD_Click(sender As Object, e As EventArgs) Handles btnDate.Click
        sendString("d")
    End Sub

    Private Sub btnT_Click(sender As Object, e As EventArgs) Handles btnTime.Click
        sendString("t")
    End Sub


    Private Sub relayChange(relayNumber As Integer)
        sendString("I" & relayPort(relayNumber) & If(relayStatus(relayNumber), ":1", ":0"))
        relayStatus(relayNumber) = Not relayStatus(relayNumber)
    End Sub

    Private Sub btnRelay1_Click(sender As Object, e As EventArgs) Handles btnRelay1.Click
        relayChange(0)
    End Sub
    Private Sub btnRelay2_Click(sender As Object, e As EventArgs) Handles btnRelay2.Click
        relayChange(1)
    End Sub
    Private Sub btnRelay3_Click(sender As Object, e As EventArgs) Handles btnRelay3.Click
        relayChange(2)
    End Sub
    Private Sub btnRelay4_Click(sender As Object, e As EventArgs) Handles btnRelay4.Click
        relayChange(3)
    End Sub
    Private Sub btnRelay5_Click(sender As Object, e As EventArgs) Handles btnRelay5.Click
        relayChange(4)
    End Sub
    Private Sub btnRelay6_Click(sender As Object, e As EventArgs) Handles btnRelay6.Click
        relayChange(5)
    End Sub
    Private Sub btnRelay7_Click(sender As Object, e As EventArgs) Handles btnRelay7.Click
        relayChange(6)
    End Sub
    Private Sub btnRelay8_Click(sender As Object, e As EventArgs) Handles btnRelay8.Click
        relayChange(7)
    End Sub

    Private Sub btnTest_Click(sender As Object, e As EventArgs) Handles btnTest.Click
        If SerialPort1.IsOpen = True Then
            SerialPort1.Write("@04qc")          ' This is a truncated command with no checksum
        End If
    End Sub

    Private Sub Timer1_Tick_1(sender As Object, e As EventArgs) Handles Timer1.Tick
        If SerialPort1.IsOpen = True Then
            Try
                sendString("t")  'Send the time command every 5 minutes
            Catch ex As Exception
                btnConnect.Enabled = True
                btnDisconnect.Enabled = False
                btnSend.Enabled = False
            End Try
        End If
    End Sub


    Private Sub btnTemp0_Click(sender As Object, e As EventArgs) Handles btnTemp0.Click
        sendString("T0")
    End Sub
    Private Sub btnTemp1_Click(sender As Object, e As EventArgs) Handles btnTemp1.Click
        sendString("T1")
    End Sub
    Private Sub btnTemp6_Click(sender As Object, e As EventArgs) Handles btnTemp6.Click
        sendString("T6")
    End Sub
    Private Sub btnTemp7_Click(sender As Object, e As EventArgs) Handles btnTemp7.Click
        sendString("T7")
    End Sub
    Private Sub btnTempClear_Click(sender As Object, e As EventArgs) Handles btnTempClear.Click
        sendString("T0:0")
    End Sub

    Private Sub btnLeds1_Click(sender As Object, e As EventArgs) Handles btnLeds1.Click
        sendString("L1:")
    End Sub
    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles btnLeds2.Click
        sendString("L2:")
    End Sub
    Private Sub btnLeds3_Click(sender As Object, e As EventArgs) Handles btnLeds3.Click
        sendString("L3:")
    End Sub
    Private Sub btnLeds4_Click(sender As Object, e As EventArgs) Handles btnLeds4.Click
        sendString("L4:")
    End Sub
    Private Sub btnLeds5_Click(sender As Object, e As EventArgs) Handles btnLeds5.Click
        sendString("L5:")
    End Sub
    Private Sub btnLeds6_Click(sender As Object, e As EventArgs) Handles btnLeds6.Click
        sendString("L6:")
    End Sub
    Private Sub btnLeds7_Click(sender As Object, e As EventArgs) Handles btnLeds7.Click
        sendString("L7:")
    End Sub
    Private Sub btnLeds8_Click(sender As Object, e As EventArgs) Handles btnLeds8.Click
        sendString("L8:")
    End Sub
    Private Sub btnLedsOff_Click(sender As Object, e As EventArgs) Handles btnLedsOff.Click
        sendString("L0,50,0")
    End Sub

    Private Sub btnCounter2_Click(sender As Object, e As EventArgs) Handles btnCounter2.Click
        sendString("i0")
    End Sub
    Private Sub btnCounter3_Click(sender As Object, e As EventArgs) Handles btnCounter3.Click
        sendString("i1")
    End Sub

    Private Sub btnBeep1_Click(sender As Object, e As EventArgs) Handles btnBeep1.Click
        sendString("b1")
    End Sub
    Private Sub btnBeep51_Click(sender As Object, e As EventArgs) Handles btnBeep51.Click
        sendString("b51")
    End Sub

    Private Sub btnAnalog0_Click(sender As Object, e As EventArgs) Handles btnAnalog0.Click
        sendString("a0")
    End Sub
    Private Sub btnAnalog1_Click(sender As Object, e As EventArgs) Handles btnAnalog1.Click
        sendString("a1")
    End Sub
    Private Sub btnAnalog2_Click(sender As Object, e As EventArgs) Handles btnAnalog2.Click
        sendString("a2")
    End Sub
    Private Sub btnAnalog3_Click(sender As Object, e As EventArgs) Handles btnAnalog3.Click
        sendString("a3")
    End Sub
    Private Sub btnAV_Click(sender As Object, e As EventArgs) Handles btnAV.Click
        sendString("a4")
    End Sub
    Private Sub btnAT_Click(sender As Object, e As EventArgs) Handles btnAT.Click
        sendString("a5")
    End Sub
    Private Sub btnAnalog6_Click(sender As Object, e As EventArgs) Handles btnAnalog6.Click
        sendString("a6")
    End Sub
    Private Sub btnAnalog7_Click(sender As Object, e As EventArgs) Handles btnAnalog7.Click
        sendString("a7")
    End Sub
    Private Sub btnAnalog8_Click(sender As Object, e As EventArgs) Handles btnAnalog8.Click
        sendString("a8")
    End Sub
    Private Sub btnAnalogClear_Click(sender As Object, e As EventArgs) Handles btnAnalogClear.Click
        sendString("a0:")
    End Sub

    Private Sub btnVersion_Click(sender As Object, e As EventArgs) Handles btnVersion.Click
        sendString("v")
    End Sub

    Private Sub btnAlarm0off_Click(sender As Object, e As EventArgs) Handles btnAlarm0off.Click
        sendString("A0:0")
    End Sub
    Private Sub btnAlarm1off_Click(sender As Object, e As EventArgs) Handles btnAlarm1off.Click
        sendString("A1:0")
    End Sub
    Private Sub btnAlarm2off_Click(sender As Object, e As EventArgs) Handles btnAlarm2off.Click
        sendString("A2:0")
    End Sub
    Private Sub btnAlarm3off_Click(sender As Object, e As EventArgs) Handles btnAlarm3off.Click
        sendString("A3:0")
    End Sub

    Private Sub btnAlarm0on_Click(sender As Object, e As EventArgs) Handles btnAlarm0on.Click
        sendString("A0:1")
    End Sub
    Private Sub btnAlarm1on_Click(sender As Object, e As EventArgs) Handles btnAlarm1on.Click
        sendString("A1:1")
    End Sub
    Private Sub btnAlarm2on_Click(sender As Object, e As EventArgs) Handles btnAlarm2on.Click
        sendString("A2:1")
    End Sub
    Private Sub btnAlarm3on_Click(sender As Object, e As EventArgs) Handles btnAlarm3on.Click
        sendString("A3:1")
    End Sub

    Private Sub btnAlarm0_Click(sender As Object, e As EventArgs) Handles btnAlarm0.Click
        sendString("A0")
    End Sub
    Private Sub btnAlarm1_Click(sender As Object, e As EventArgs) Handles btnAlarm1.Click
        sendString("A1")
    End Sub
    Private Sub btnAlarm2_Click(sender As Object, e As EventArgs) Handles btnAlarm2.Click
        sendString("A2")
    End Sub
    Private Sub btnAlarm3_Click(sender As Object, e As EventArgs) Handles btnAlarm3.Click
        sendString("A3")
    End Sub

    Private Sub Timer2_Tick(sender As Object, e As EventArgs) Handles Timer2.Tick
        lblDateTime.Text = Format(Now, "dd/MM H:mm:ss")
    End Sub

    Private Sub calc_alarm_changed()
        Dim AlarmSecondsMax = 60 * 60  ' <3600 = timer mode in 1 second intervals up to one hour
        Dim AlarmTenSecondsMax = 16384 ' timer mode in 10 second intervals from one hour to approx 30 hours
        Dim AlarmOnSecondEveryMinuteMax = 60 ' alarm on the nth second of (every) minute (once per minute)
        Dim AlarmOnMinuteEveryHourMax = 60 ' alarm on the 0th second of the nth minute of (every) hour (once per hour)
        Dim AlarmOnMinuteEveryDayMax = 60 * 24 ' alarm on the 0th second of the nth minute of the nth hour daily (once per day)
        Dim AlarmRepeatBit = 1 << 15

        Dim time_array() As String
        Dim seconds As Integer
        If (Len(txtAlarmInput.Text)) Then
            time_array = Split(txtAlarmInput.Text, ":")
            seconds = CInt(time_array(0))
            If UBound(time_array) >= 1 Then
                seconds *= 60
                seconds += CInt(time_array(1))
            End If
            If UBound(time_array) >= 2 Then
                seconds *= 60
                seconds += CInt(time_array(2))
            End If
            If lblTimerOrAlarm.Text = "Timer" Then
                If seconds > AlarmSecondsMax Then
                    seconds = (seconds - AlarmSecondsMax) \ 10 + AlarmSecondsMax
                End If
            Else ' lblTimerOrAlarm.Text = "Alarm"
                If UBound(time_array) >= 2 Then
                    seconds = seconds \ 60 + AlarmOnSecondEveryMinuteMax + AlarmOnMinuteEveryHourMax
                ElseIf UBound(time_array) >= 1 Then
                    seconds = seconds \ 60 + AlarmOnSecondEveryMinuteMax
                End If
                seconds += AlarmTenSecondsMax
            End If

            If lblAutoRepeat.Text = "Auto Repeat" Then
                seconds += AlarmRepeatBit
            End If

            lblAlarmValues.Text = CStr(seconds Mod 256) + "," + CStr(Int(seconds \ 256))
        End If
    End Sub


    Private Sub txtAlarmInput_TextChanged(sender As Object, e As EventArgs) Handles txtAlarmInput.TextChanged
        calc_alarm_changed()
    End Sub

    Private Sub lblTimerOrAlarm_Click(sender As Object, e As EventArgs) Handles lblTimerOrAlarm.Click
        If (lblTimerOrAlarm.Text = "Timer") Then
            lblTimerOrAlarm.Text = "Alarm"
        Else
            lblTimerOrAlarm.Text = "Timer"
        End If
        calc_alarm_changed()
    End Sub

    Private Sub lblAutoRepeat_Click(sender As Object, e As EventArgs) Handles lblAutoRepeat.Click
        If (lblAutoRepeat.Text = "Auto Repeat") Then
            lblAutoRepeat.Text = "No Repeat"
        Else
            lblAutoRepeat.Text = "Auto Repeat"
        End If
        calc_alarm_changed()
    End Sub


    Private Sub txtDeviceId_TextChanged(sender As Object, e As EventArgs) Handles txtDeviceId.TextChanged
        deviceId = Convert.ToInt32(txtDeviceId.Text)
    End Sub



End Class
'Whole Code Ends Here ….