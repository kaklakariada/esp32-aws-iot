import * as cdk from '@aws-cdk/core';
import * as iot from '@aws-cdk/aws-iot';

const CERT_ARN = 'arn:aws:iot:eu-west-1:039638555911:cert/07693e715764281a03d5d7922aca23d790e515c99bc3c192e8cbd625695fdfb4';

export class InfrastructureStack extends cdk.Stack {
  constructor(scope: cdk.Construct, id: string, props?: cdk.StackProps) {
    super(scope, id, props);

    const thingName = 'ESP32Sensor';
    const policyName = 'ESP32policy';

    const thing = new iot.CfnThing(this, 'esp32', {
      thingName: thingName,
      attributePayload: {}
    });

    new iot.CfnThingPrincipalAttachment(this, 'thingPrincipalAttachmet', {
      principal: CERT_ARN,
      thingName: thingName
    });

    const policy = new iot.CfnPolicy(this, 'thingPolicy', {
      policyName: policyName,
      policyDocument: {
        Version: "2012-10-17",
        Statement: [
          {
            Effect: "Allow",
            Action: "iot:Connect",
            Resource: "*"
          },
          {
            Effect: "Allow",
            Action: "iot:Publish",
            Resource: "*"
          },
          {
            Effect: "Allow",
            Action: "iot:Subscribe",
            Resource: "*"
          },
          {
            Effect: "Allow",
            Action: "iot:Receive",
            Resource: "*"
          }
        ]
      }
    });

    new iot.CfnPolicyPrincipalAttachment(this, 'policyPrincipalAttachment', {
      policyName: policyName,
      principal: CERT_ARN
    });
  }
}
