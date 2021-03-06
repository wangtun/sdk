// Copyright (c) 2017, the Dart project authors.  Please see the AUTHORS file
// for details. All rights reserved. Use of this source code is governed by a
// BSD-style license that can be found in the LICENSE file.

import "dart:io";

void main(List<String> args) {
  if (args.contains("--child")) {
    print("Hello, pre-scanned world!");
  } else {
    generateSnapshot();
  }
}

void generateSnapshot() {
  var tempDir = Directory.systemTemp.createTempSync("script-snapshot");
  var snapshotPath = tempDir.uri.resolve("hello.snapshot").toFilePath();
  var depfilePath = tempDir.uri.resolve("hello.snapshot.d").toFilePath();
  var scriptPath = Platform.script.toFilePath();

  var exec = Platform.resolvedExecutable;
  var args = new List();
  args.addAll(Platform.executableArguments);
  args.add("--snapshot=$snapshotPath");
  args.add("--snapshot-depfile=$depfilePath");
  args.add(scriptPath);
  args.add("--child");
  var result = Process.runSync(exec, args);

  print("Exit code: ${result.exitCode}");
  print("stdout:");
  print(result.stdout);
  print("stderr:");
  print(result.stderr);

  if (result.exitCode != 0) {
    throw "Bad exit code: ${result.exitCode}";
  }

  var depfileContents = new File(depfilePath).readAsStringSync();
  print("depfile:");
  print(depfileContents);
  if (!depfileContents.contains(snapshotPath)) {
    print("snapshotPath:");
    print(snapshotPath);
    throw "Missing snapshot path";
  }
  if (!depfileContents.contains(scriptPath)) {
    print("scriptPath:");
    print(scriptPath);
    throw "Missing script path";
  }
}
