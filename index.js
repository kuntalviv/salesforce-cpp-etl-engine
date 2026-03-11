require("dotenv").config();
const jsforce = require('jsforce');
const etl = require('./build/Release/process_data');

const { SF_LOGIN_URL, SF_USERNAME, SF_PASSWORD, SF_TOKEN } = process.env;


async function authenticate() {
  const conn = new jsforce.Connection({
    loginUrl: SF_LOGIN_URL
  });

  console.log(SF_USERNAME + " " + SF_PASSWORD + " " + SF_TOKEN);
  try {
    await conn.login(SF_USERNAME, SF_PASSWORD + SF_TOKEN);
  } catch (e) {
    throw new Error(e.message);
  }

  return conn;
}

async function queryRecords(conn) {
  let records;
  try {
    records = await conn.query("SELECT Id, Name FROM Account");
  } catch (e) {
    throw new Error(e.message);
  }
  return records;
}

async function sendRecordsToETL(records, method) {
  const transformedRecords = etl.processData(records.records, method);
  return transformedRecords;
}

async function pushRecordsToSF(conn, records) {
  //For large set
  // conn.bulk.pollTimeout = 300000;
  // conn.bulk.pollInterval = 5000;
  // conn.bulk.load("Account", "update", records, (err, rets) => {
  //   if (err) throw new Error(err);
  //   else return "Success";
  // });

  //For small set
  try {
    await conn.sobject("Account").upsert(records, 'Id', { allowRecursive: true })
  } catch (e) {
    throw new Error(e.message);
  }

}

async function main() {
  console.log("Starting main");
  let conn;
  try {
    conn = await authenticate();
    console.log("Successfully Authenticated");
  } catch (e) {
    console.error("Error: " + e.message);
    return;
  }

  let records;

  try {
    records = await queryRecords(conn);
  } catch (e) {
    console.error("Error:" + e.message);
    return;
  }

  let transformedRecords;
  try {
    transformedRecords = await sendRecordsToETL(records, "toLowerCase");
    console.log(transformedRecords);
  } catch (e) {
    console.error("Error:" + e.message);
    return;

  }

  let status;
  try {
    status = await pushRecordsToSF(conn, transformedRecords);
  } catch (e) {
    console.log("Error: " + e.message);
  }

}

main();

